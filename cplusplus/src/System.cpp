#include "System.hpp"

#include "Console.hpp"
#include <stdarg.h>

#include <iostream>
#include <cstring>
#include <wchar.h>

using namespace cpp;
using namespace std;
using namespace uniconv;

nstring System::self = System::GetSelf();
nstring System::root = System::GetRoot();

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef _MSC_VER
    #define PATH_MAX MAX_PATH
#endif
    nstring System::GetSelf(void) {
        if (System::self.size()) return System::self;
        wchar_t buf[PATH_MAX];
        DWORD size = GetModuleFileName(NULL, buf, PATH_MAX);
        return buf;
    }

    nstring System::GetRoot(void) {
        if (!self.size()) System::self = System::GetSelf();
        wchar_t edit[PATH_MAX];
        auto size = System::self.size();
        while (System::self[--size] != '\\')
        for (size_t i = 0; i <= size; i++) edit[i] = System::self[i];
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        nstring out = nstring(edit);
        return out;
    }

    nstring System::ToNativePath(nstring path) {
        for (int i = 0; path[i] != L'\0'; i++) if (path[i] == L'/') path[i] = L'\\';
        return path;
    }

    
    int System::ClearDirectory(uniconv::utfcstr path) {
        wstring folderPath = path;
        if (folderPath.back() != L'\\') folderPath.push_back(L'\\');
        folderPath.push_back(L'*');
        WIN32_FIND_DATA info;
        HANDLE FindHndle = FindFirstFileEx(folderPath.c_str(), FindExInfoBasic, &info, FindExSearchNameMatch, NULL, 0);
        if (!FindHndle) return GetLastError();
        do {
            wstring fileFound = path;
            if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(info.cFileName, L".") == 0 || wcscmp(info.cFileName, L"..") == 0) continue;
                if (fileFound.back() != L'\\') fileFound.push_back(L'\\');
                fileFound.append(info.cFileName);
                auto res = ClearDirectory(fileFound.c_str());
                if (res) return res;
                res = DeleteDirectory(fileFound.c_str());
                if (res) return res;
                continue;
            }
            if (fileFound.back() != L'\\') fileFound.push_back(L'\\');
            fileFound.append(info.cFileName);
            auto res = RemoveFile(fileFound.c_str());
            if (res) return res;
        } while(FindNextFile(FindHndle, &info));
        if (FindClose(FindHndle)) return GetLastError();
        return 0;
    }

    int System::DeleteDirectory(uniconv::utfcstr path) {
        if (RemoveDirectory(path)) return 0;
        return GetLastError();
    }

    int System::RemoveFile(uniconv::utfcstr path) {
        if (DeleteFile(path)) return 0;
        return GetLastError();
    }

    bool System::IsFile(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool System::IsDirectory(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool System::DoesPathExist(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES);
    }

    unordered_map<fd_t, pair<pair<HANDLE, tsqueue<wstring>>, wstring>> System::pipes{};

    DWORD WINAPI PipeBackendThread(LPVOID param) {
        auto params = static_cast<pair<HANDLE, tsqueue<wstring>*>*>(param);
        auto [pipe, messages] = *params;
        delete params;
        
        if (!ConnectNamedPipe(pipe, nullptr)) {
            auto err = GetLastError();
            if (err == ERROR_PIPE_CONNECTED) goto allgood;
            //std::wcerr << L"Failed to connect to the client. Error: " << GetLastError() << std::endl;
            CloseHandle(pipe);
            return 1;
        }
    allgood:
        
        while (1) {
            if (!messages->empty()) {
                wstring message = messages->pop();
                //wcerr << L"Writing: " << message << endl;
                DWORD bytesWritten;
                if (!WriteFile(pipe, message.c_str(), message.size() * sizeof(wchar_t), &bytesWritten, nullptr))
                    return 1;
            } else {
                WaitMessage();
                MSG msg;
                while (auto val = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    if (val < 0) return 1;
            }
        }
        return 0;
    }

    fd_t System::CreatePipe(uniconv::utfcstr subpath)
    {
        wstring fullpath = L"\\\\.\\pipe\\";
        fullpath.append(subpath);
        HANDLE pipe = CreateNamedPipeW(fullpath.c_str(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 0, 0, 0, nullptr);

        if (pipe == INVALID_HANDLE_VALUE || pipe == INVALID_HANDLE_VALUE)
            return INVALID_HANDLE_VALUE;
        
        if (!CreateSymbolicLink(subpath, fullpath.c_str(), SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)) {
            auto err = GetLastError();
            if (err == ERROR_INVALID_PARAMETER) { // Windows 10 build < 14972 doesnt support `SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE`
                if (CreateSymbolicLink(subpath, fullpath.c_str(), 0))
                    goto good;
                err = GetLastError();
            }
            Console::ThrowMsg(wstring(L"CreateSymbolicLink failed: ") + fullpath + L", with error: " + to_wstring(err));
        }
    good:

        System::pipes[pipe];
        System::pipes[pipe].second = subpath;
        System::pipes[pipe].first.first = CreateThread(NULL, 0, PipeBackendThread, new pair<HANDLE, tsqueue<wstring>*>(pipe, &(System::pipes[pipe].first.second)), 0, NULL);
        return pipe;
    }

    fd_t System::OpenPipe(uniconv::utfcstr subpath) {
        //wstring fullpath = L"\\\\.\\pipe\\";
        //fullpath.append(subpath);
        HANDLE pipe = CreateFile(subpath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;
        return pipe;
    }

    void System::ClosePipe(fd_t pipe) {
        if (System::pipes.find(pipe) != System::pipes.end()) {
            TerminateThread(System::pipes[pipe].first.first, 0);
            WaitForSingleObject(System::pipes[pipe].first.first, INFINITE);
            CloseHandle(System::pipes[pipe].first.first);
            RemoveFile(System::pipes[pipe].second.c_str());
            System::pipes.erase(pipe);
        }
        CloseHandle(pipe);
    }

    int System::SendMessagePipe(fd_t pipe, uniconv::nstring msg) {
        if (System::pipes.find(pipe) == System::pipes.end())
            return ERROR_ACCESS_DENIED; // read device
        if (msg.size() > 255) wcerr << L"Message: \"" << msg << "\" greater than 255 bytes" << std::endl;
        System::pipes[pipe].first.second.push(msg);
        PostThreadMessage(GetThreadId(System::pipes[pipe].first.first), WM_USER, 0, 0);
        return 0;
    }

    uniconv::nstring System::ReadMessagePipe(fd_t pipe) {
        if (System::pipes.find(pipe) != System::pipes.end())
            return wstring(); // write device
        
        wchar_t buf[256];
        DWORD siz = 0;
        if (!ReadFile(pipe, buf, sizeof(buf) - sizeof(wchar_t), &siz, nullptr))
            return wstring();
        buf[siz/sizeof(wchar_t)] = L'\0';
        return buf;
    }

    int System::MakeDirectory(utfcstr path) {
        //wcerr << L"MakeDirectory: " << path << endl; 
        SECURITY_ATTRIBUTES sec_atrs{};
        sec_atrs.nLength = sizeof(SECURITY_ATTRIBUTES);
        sec_atrs.lpSecurityDescriptor = nullptr;
        sec_atrs.bInheritHandle = false;
        if (CreateDirectory(path, &sec_atrs)) return 0;
        return GetLastError(); 
    }

    int System::Shell(uniconv::utfcstr arg) {
        return System::RunProgram(L"C:\\Windows\\System32\\cmd.exe", L"/c", arg, nullptr);
    }

    bool System::ShellAsync(uniconv::utfcstr arg) {
        return System::RunProgramAsync(L"C:\\Windows\\System32\\cmd.exe", L"/c", arg, nullptr);
    }

    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramC
    int System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        va_start(args, arg);
        args_v += L" \"";
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
        va_end(args);
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return -1;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        bool isdone = false;
        DWORD exitcode = -1;
        if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
            CloseHandle(pi.hProcess);
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    int System::RunProgramC(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        va_start(args, arg);
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
        va_end(args);
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return -1;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NEW_CONSOLE;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x75); }
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        bool isdone = false;
        DWORD exitcode = -1;
        if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
            CloseHandle(pi.hProcess);
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    int System::RunProgram0(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        va_start(args, arg);
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
        va_end(args);
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return -1;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NO_WINDOW;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x75); }
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        bool isdone = false;
        DWORD exitcode = -1;
        if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
            CloseHandle(pi.hProcess);
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    
    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramAsyncC
    bool System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        va_start(args, arg);
        args_v += L" \"";
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
        va_end(args);
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return false;
        }

        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;
        //wcerr << path << L' ' << args_v << endl;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    bool System::RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        va_start(args, arg);
        args_v += L" \"";
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
        va_end(args);
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return false;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NEW_CONSOLE;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x75); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    int System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return -1;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        bool isdone = false;
        DWORD exitcode = -1;
        if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
            CloseHandle(pi.hProcess);
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    int System::RunProgramC(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return -1;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NEW_CONSOLE;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x75); }
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        bool isdone = false;
        DWORD exitcode = -1;
        if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
            CloseHandle(pi.hProcess);
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    
    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramAsyncC
    bool System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return false;
        }

        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;
        //wcerr << path << L' ' << args_v << endl;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    bool System::RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            wstring argx_w = wstring(argx);
            wstring argx_vv;
            for (const wchar_t& c : argx_w)
                if (c == L'\"') argx_vv += L"\\\""; else argx_vv += c;
            args_v.append(L"\" \"").append(argx_vv);
        }
        args_v += L"\"";
    noargs:
        if (!PathFileExists(path) && !PathFileExists((path + wstring(L".exe")).c_str())) {
            return false;
        }
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NEW_CONSOLE;
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x75); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    
#else
#include <sys/stat.h>
#ifdef __APPLE__
#define sighandler_t sig_t
#endif

int System::DeleteDirectory(utfcstr path) {
    if (rmdir(path) == 0) return 0;
    return errno;
}

int System::RemoveFile(utfcstr path) {
    if (unlink(path) == 0) return 0;
    return errno;
}

int System::ClearDirectory(utfcstr path) {
    string spath = path;
    if (spath.back() != L'/') spath.push_back(L'/');
    DIR* dir = opendir(path);
    if (!dir) return errno;
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        if (ent->d_type == DT_DIR) {
            auto res = ClearDirectory((spath + ent->d_name).c_str());
            if (res) return res;
            res = DeleteDirectory((spath + ent->d_name).c_str());
            if (res) return res;
        } else if (int res = RemoveFile((spath + ent->d_name).c_str())) return res;
    }
    closedir(dir);
    return 0;
}

bool System::IsFile(utfcstr path) {
    struct stat st;
    if (stat(path, &st) == 0) return S_ISREG(st.st_mode);
    return false;
}

bool System::IsDirectory(utfcstr path) {
    struct stat st;
    if (stat(path, &st) == 0) return S_ISDIR(st.st_mode);
    return false;
}

bool cpp::System::DoesPathExist(utfcstr path) {
    struct stat st;
    return stat(path, &st) == 0; 
}

int System::MakeDirectory(utfcstr path) {
    if (mkdir(path, 0777) == 0) return 0;
    return errno;
}


unordered_map<fd_t, string> System::pipes{};

fd_t System::CreatePipe(uniconv::utfcstr subpath)
{
    int createpipe = mkfifo(subpath, 0600);
    if (createpipe < 0) {
        Console::out << L"Couldn't create pipe: " << subpath << L", with error: " << errno; Console::out_endl();
        return INVALID_HANDLE_VALUE;
    }
    int pipe = open(subpath, O_RDWR); // not only write, so it doesn't block
    if (pipe < 0) { Console::out << L"Couldn't open pipe: " << subpath << L", with error: " << errno; Console::out_endl(); }
    else pipes[pipe] = subpath;
    return pipe;
}

fd_t System::OpenPipe(uniconv::utfcstr subpath) {
    int pipe = open(subpath, O_RDONLY);
    if (pipe < 0) { Console::out << L"Couldn't open pipe: " << subpath << L", with error: " << errno; Console::out_endl(); }
    return pipe;
}

void System::ClosePipe(fd_t pipe) {
    close(pipe);
    if (pipes.find(pipe) != pipes.end()) {
        unlink(pipes[pipe].c_str());
        pipes.erase(pipe);
    }
}

struct pipemsg { size_t size; char* buf; };

int send_pipemsg(int fd, pipemsg msg) {
    int ret = write(fd, &(msg.size), sizeof(msg.size));
    if (ret < 0) return errno;
    ret = write(fd, msg.buf, msg.size);
    if (ret < 0) return errno;
    return 0;
}

int read_pipemsg(int fd, pipemsg* msg) {
    int ret = read(fd, &(msg->size), sizeof(msg->size));
    if (ret < 0) return errno;
    msg->buf = (char*)malloc(msg->size);
    ret = read(fd, msg->buf, msg->size);
    if (ret < 0) return errno;
    return 0;
}

int System::SendMessagePipe(fd_t pipe, uniconv::nstring msg) {
    pipemsg send = {msg.size(), (char*)msg.c_str()};
    return send_pipemsg(pipe, send);
}

string System::ReadMessagePipe(fd_t pipe) {
    if (pipes.find(pipe) != pipes.end()) return string(); // not a read pipe

    pipemsg recv;
    int ret = read_pipemsg(pipe, &recv);
    if (ret != 0){
        Console::out << L"Couldn't read message from pipe " << pipe << L"wit error: " << ret; Console::out_endl();
        return string();
    }
    string out(recv.buf, recv.size);
    free(recv.buf);
    return out;
}

int System::Shell(uniconv::utfcstr command) {
    return System::RunProgram("/bin/sh", "-c", command, nullptr); 
}

bool System::ShellAsync(uniconv::utfcstr command) {
    return System::RunProgramAsync("/bin/sh", "-c", command, nullptr);
}

#define CALL_RETRY(retvar, expression) do { \
    retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

pid_t System::tpid = 0;

void System::SendSignal(int signal) {
    kill(tpid, signal);
}

bool System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
    int status = 0;
    System::tpid = fork();
    sighandler_t old_handler[32];
    if (tpid < 0)
        return false;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        va_list args;
        va_start(args, arg);
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        if (arg == nullptr) goto noargs;
        args_c[1] = (char*)arg;
        for (int i = 2; i < 64; i++) {
            const char* argx = va_arg(args, const char*);
            if (argx == nullptr) break;
            args_c[i] = (char*)argx;
        }
    noargs:
        va_end(args);

        execv(path, args_c);
        exit(127);
    }
    return true;
}

int System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
    int status;
    System::tpid = fork();
    sighandler_t old_handler[32];
    if (tpid < 0)
        status = -1;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        va_list args;
        va_start(args, arg);
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        if (arg == nullptr) goto noargs;
        args_c[1] = (char*)arg;
        for (int i = 2; i < 64; i++) {
            const char* argx = va_arg(args, const char*);
            if (argx == nullptr) break;
            args_c[i] = (char*)argx;
        }
    noargs:
        va_end(args);

        execv(path, args_c);
        exit(127);
    } else {
        old_handler[SIGHUP] = signal(SIGHUP, System::SendSignal);
        old_handler[SIGINT] = signal(SIGINT, System::SendSignal);
        old_handler[SIGQUIT] = signal(SIGQUIT, System::SendSignal);
        old_handler[SIGILL] = signal(SIGILL, System::SendSignal);
        old_handler[SIGTRAP] = signal(SIGTRAP, System::SendSignal);
        old_handler[SIGABRT] = signal(SIGABRT, System::SendSignal);
        old_handler[SIGIOT] = signal(SIGIOT, System::SendSignal);
        old_handler[SIGFPE] = signal(SIGFPE, System::SendSignal);
        old_handler[SIGKILL] = signal(SIGKILL, System::SendSignal);
        old_handler[SIGUSR1] = signal(SIGUSR1, System::SendSignal);
        old_handler[SIGSEGV] = signal(SIGSEGV, System::SendSignal);
        old_handler[SIGUSR2] = signal(SIGUSR2, System::SendSignal);
        old_handler[SIGPIPE] = signal(SIGPIPE, System::SendSignal);
        old_handler[SIGTERM] = signal(SIGTERM, System::SendSignal);
    #ifdef SIGSTKFLT
        old_handler[SIGSTKFLT] = signal(SIGSTKFLT, System::SendSignal);
    #endif
        old_handler[SIGCHLD] = signal(SIGCHLD, System::SendSignal);
        old_handler[SIGCONT] = signal(SIGCONT, System::SendSignal);
        old_handler[SIGSTOP] = signal(SIGSTOP, System::SendSignal);
        old_handler[SIGTSTP] = signal(SIGTSTP, System::SendSignal);
        old_handler[SIGTTIN] = signal(SIGTTIN, System::SendSignal);
        old_handler[SIGTTOU] = signal(SIGTTOU, System::SendSignal);
        int wexit;
        pid_t ret; CALL_RETRY(ret,waitpid(tpid, &wexit, 0))
        if (ret != tpid)
            return -1;
        if (!WIFEXITED(wexit))
            return -1;
        status = WEXITSTATUS(wexit);
    }
    signal(SIGHUP, old_handler[SIGHUP]);
    signal(SIGINT, old_handler[SIGINT]);
    signal(SIGQUIT, old_handler[SIGQUIT]);
    signal(SIGILL, old_handler[SIGILL]);
    signal(SIGTRAP, old_handler[SIGTRAP]);
    signal(SIGABRT, old_handler[SIGABRT]);
    signal(SIGIOT, old_handler[SIGIOT]);
    signal(SIGFPE, old_handler[SIGFPE]);
    signal(SIGKILL, old_handler[SIGKILL]);
    signal(SIGUSR1, old_handler[SIGUSR1]);
    signal(SIGSEGV, old_handler[SIGSEGV]);
    signal(SIGUSR2, old_handler[SIGUSR2]);
    signal(SIGPIPE, old_handler[SIGPIPE]);
    signal(SIGTERM, old_handler[SIGTERM]);
#ifdef SIGSTKFLT
    signal(SIGSTKFLT, old_handler[SIGSTKFLT]);
#endif
    signal(SIGCHLD, old_handler[SIGCHLD]);
    signal(SIGCONT, old_handler[SIGCONT]);
    signal(SIGSTOP, old_handler[SIGSTOP]);
    signal(SIGTSTP, old_handler[SIGTSTP]);
    signal(SIGTTIN, old_handler[SIGTTIN]);
    signal(SIGTTOU, old_handler[SIGTTOU]);
    return status;
}

int System::RunProgramS(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
    int status;
    System::tpid = fork();
    sighandler_t old_handler[32];
    if (tpid < 0)
        status = -1;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        va_list args;
        va_start(args, arg);
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        if (arg == nullptr) goto noargs;
        args_c[1] = (char*)arg;
        for (int i = 2; i < 256; i++) {
            const char* argx = va_arg(args, const char*);
            if (argx == nullptr) break;
            args_c[i] = (char*)argx;
        }
    noargs:
        va_end(args);

        execvp(path, args_c);
        exit(127);
    } else {
        old_handler[SIGHUP] = signal(SIGHUP, System::SendSignal);
        old_handler[SIGINT] = signal(SIGINT, System::SendSignal);
        old_handler[SIGQUIT] = signal(SIGQUIT, System::SendSignal);
        old_handler[SIGILL] = signal(SIGILL, System::SendSignal);
        old_handler[SIGTRAP] = signal(SIGTRAP, System::SendSignal);
        old_handler[SIGABRT] = signal(SIGABRT, System::SendSignal);
        old_handler[SIGIOT] = signal(SIGIOT, System::SendSignal);
        old_handler[SIGFPE] = signal(SIGFPE, System::SendSignal);
        old_handler[SIGKILL] = signal(SIGKILL, System::SendSignal);
        old_handler[SIGUSR1] = signal(SIGUSR1, System::SendSignal);
        old_handler[SIGSEGV] = signal(SIGSEGV, System::SendSignal);
        old_handler[SIGUSR2] = signal(SIGUSR2, System::SendSignal);
        old_handler[SIGPIPE] = signal(SIGPIPE, System::SendSignal);
        old_handler[SIGTERM] = signal(SIGTERM, System::SendSignal);
    #ifdef SIGSTKFLT
        old_handler[SIGSTKFLT] = signal(SIGSTKFLT, System::SendSignal);
    #endif
        old_handler[SIGCHLD] = signal(SIGCHLD, System::SendSignal);
        old_handler[SIGCONT] = signal(SIGCONT, System::SendSignal);
        old_handler[SIGSTOP] = signal(SIGSTOP, System::SendSignal);
        old_handler[SIGTSTP] = signal(SIGTSTP, System::SendSignal);
        old_handler[SIGTTIN] = signal(SIGTTIN, System::SendSignal);
        old_handler[SIGTTOU] = signal(SIGTTOU, System::SendSignal);
        int wexit;
        pid_t ret; CALL_RETRY(ret,waitpid(tpid, &wexit, 0))
        if (ret != tpid)
            return -1;
        if (!WIFEXITED(wexit))
            return -1;
        status = WEXITSTATUS(wexit);
    }
    signal(SIGHUP, old_handler[SIGHUP]);
    signal(SIGINT, old_handler[SIGINT]);
    signal(SIGQUIT, old_handler[SIGQUIT]);
    signal(SIGILL, old_handler[SIGILL]);
    signal(SIGTRAP, old_handler[SIGTRAP]);
    signal(SIGABRT, old_handler[SIGABRT]);
    signal(SIGIOT, old_handler[SIGIOT]);
    signal(SIGFPE, old_handler[SIGFPE]);
    signal(SIGKILL, old_handler[SIGKILL]);
    signal(SIGUSR1, old_handler[SIGUSR1]);
    signal(SIGSEGV, old_handler[SIGSEGV]);
    signal(SIGUSR2, old_handler[SIGUSR2]);
    signal(SIGPIPE, old_handler[SIGPIPE]);
    signal(SIGTERM, old_handler[SIGTERM]);
#ifdef SIGSTKFLT
    signal(SIGSTKFLT, old_handler[SIGSTKFLT]);
#endif
    signal(SIGCHLD, old_handler[SIGCHLD]);
    signal(SIGCONT, old_handler[SIGCONT]);
    signal(SIGSTOP, old_handler[SIGSTOP]);
    signal(SIGTSTP, old_handler[SIGTSTP]);
    signal(SIGTTIN, old_handler[SIGTTIN]);
    signal(SIGTTOU, old_handler[SIGTTOU]);
    return status;
}

int System::RunProgramS(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
    int status;
    System::tpid = fork();
    sighandler_t old_handler[32];
    if (tpid < 0)
        status = -1;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        for (int i = 1; i < 256; i++)
            if (args[i-1] == nullptr) break;
            else args_c[i] = (char*)args[i-1];

        execv(path, args_c);
        exit(127);
    } else {
        old_handler[SIGHUP] = signal(SIGHUP, System::SendSignal);
        old_handler[SIGINT] = signal(SIGINT, System::SendSignal);
        old_handler[SIGQUIT] = signal(SIGQUIT, System::SendSignal);
        old_handler[SIGILL] = signal(SIGILL, System::SendSignal);
        old_handler[SIGTRAP] = signal(SIGTRAP, System::SendSignal);
        old_handler[SIGABRT] = signal(SIGABRT, System::SendSignal);
        old_handler[SIGIOT] = signal(SIGIOT, System::SendSignal);
        old_handler[SIGFPE] = signal(SIGFPE, System::SendSignal);
        old_handler[SIGKILL] = signal(SIGKILL, System::SendSignal);
        old_handler[SIGUSR1] = signal(SIGUSR1, System::SendSignal);
        old_handler[SIGSEGV] = signal(SIGSEGV, System::SendSignal);
        old_handler[SIGUSR2] = signal(SIGUSR2, System::SendSignal);
        old_handler[SIGPIPE] = signal(SIGPIPE, System::SendSignal);
        old_handler[SIGTERM] = signal(SIGTERM, System::SendSignal);
    #ifdef SIGSTKFLT
        old_handler[SIGSTKFLT] = signal(SIGSTKFLT, System::SendSignal);
    #endif
        old_handler[SIGCHLD] = signal(SIGCHLD, System::SendSignal);
        old_handler[SIGCONT] = signal(SIGCONT, System::SendSignal);
        old_handler[SIGSTOP] = signal(SIGSTOP, System::SendSignal);
        old_handler[SIGTSTP] = signal(SIGTSTP, System::SendSignal);
        old_handler[SIGTTIN] = signal(SIGTTIN, System::SendSignal);
        old_handler[SIGTTOU] = signal(SIGTTOU, System::SendSignal);
        int wexit;
        pid_t ret; CALL_RETRY(ret,waitpid(tpid, &wexit, 0))
        if (ret != tpid)
            return -1;
        if (!WIFEXITED(wexit))
            return -1;
        status = WEXITSTATUS(wexit);
    }
    signal(SIGHUP, old_handler[SIGHUP]);
    signal(SIGINT, old_handler[SIGINT]);
    signal(SIGQUIT, old_handler[SIGQUIT]);
    signal(SIGILL, old_handler[SIGILL]);
    signal(SIGTRAP, old_handler[SIGTRAP]);
    signal(SIGABRT, old_handler[SIGABRT]);
    signal(SIGIOT, old_handler[SIGIOT]);
    signal(SIGFPE, old_handler[SIGFPE]);
    signal(SIGKILL, old_handler[SIGKILL]);
    signal(SIGUSR1, old_handler[SIGUSR1]);
    signal(SIGSEGV, old_handler[SIGSEGV]);
    signal(SIGUSR2, old_handler[SIGUSR2]);
    signal(SIGPIPE, old_handler[SIGPIPE]);
    signal(SIGTERM, old_handler[SIGTERM]);
#ifdef SIGSTKFLT
    signal(SIGSTKFLT, old_handler[SIGSTKFLT]);
#endif
    signal(SIGCHLD, old_handler[SIGCHLD]);
    signal(SIGCONT, old_handler[SIGCONT]);
    signal(SIGSTOP, old_handler[SIGSTOP]);
    signal(SIGTSTP, old_handler[SIGTSTP]);
    signal(SIGTTIN, old_handler[SIGTTIN]);
    signal(SIGTTOU, old_handler[SIGTTOU]);
    return status;
}

bool System::RunProgramAsyncS(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
    System::tpid = fork();
    if (tpid < 0)
        return false;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        for (int i = 1; i < 256; i++)
            if (args[i-1] == nullptr) break;
            else args_c[i] = (char*)args[i-1];
        execvp(path, args_c);
        exit(127);
    }
    return true;
}


bool System::RunProgramAsyncS(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
    System::tpid = fork();
    if (tpid < 0)
        return false;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        va_list args;
        va_start(args, arg);
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        if (arg == nullptr) goto noargs;
        args_c[1] = (char*)arg;
        for (int i = 2; i < 64; i++) {
            const char* argx = va_arg(args, const char*);
            if (argx == nullptr) break;
            args_c[i] = (char*)argx;
        }
    noargs:
        va_end(args);
        execvp(path, args_c);
        exit(127);
    }
    return true;
}

int System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]
#ifdef __linux__
    , uid_t suid
#endif
) {
    int status;
    System::tpid = fork();
    sighandler_t old_handler[32];
    if (tpid < 0)
        status = -1;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        for (int i = 1; i < 256; i++)
            if (args[i-1] == nullptr) break;
            else args_c[i] = (char*)args[i-1];

    #ifdef __linux__
        if (suid) {
            int res = setuid(suid);
            res = setegid(getgid());
            int nulfl = open("/dev/null", O_WRONLY);
            dup2(nulfl, STDOUT_FILENO);
            dup2(nulfl, STDERR_FILENO);
            close(nulfl);
            nulfl = open("/dev/null", O_RDONLY);
            dup2(nulfl, STDIN_FILENO);
            close(nulfl);
        }
    #endif

        execv(path, args_c);
        exit(127);
    } else {
        old_handler[SIGHUP] = signal(SIGHUP, System::SendSignal);
        old_handler[SIGINT] = signal(SIGINT, System::SendSignal);
        old_handler[SIGQUIT] = signal(SIGQUIT, System::SendSignal);
        old_handler[SIGILL] = signal(SIGILL, System::SendSignal);
        old_handler[SIGTRAP] = signal(SIGTRAP, System::SendSignal);
        old_handler[SIGABRT] = signal(SIGABRT, System::SendSignal);
        old_handler[SIGIOT] = signal(SIGIOT, System::SendSignal);
        old_handler[SIGFPE] = signal(SIGFPE, System::SendSignal);
        old_handler[SIGKILL] = signal(SIGKILL, System::SendSignal);
        old_handler[SIGUSR1] = signal(SIGUSR1, System::SendSignal);
        old_handler[SIGSEGV] = signal(SIGSEGV, System::SendSignal);
        old_handler[SIGUSR2] = signal(SIGUSR2, System::SendSignal);
        old_handler[SIGPIPE] = signal(SIGPIPE, System::SendSignal);
        old_handler[SIGTERM] = signal(SIGTERM, System::SendSignal);
    #ifdef SIGSTKFLT
        old_handler[SIGSTKFLT] = signal(SIGSTKFLT, System::SendSignal);
    #endif
        old_handler[SIGCHLD] = signal(SIGCHLD, System::SendSignal);
        old_handler[SIGCONT] = signal(SIGCONT, System::SendSignal);
        old_handler[SIGSTOP] = signal(SIGSTOP, System::SendSignal);
        old_handler[SIGTSTP] = signal(SIGTSTP, System::SendSignal);
        old_handler[SIGTTIN] = signal(SIGTTIN, System::SendSignal);
        old_handler[SIGTTOU] = signal(SIGTTOU, System::SendSignal);
        int wexit;
        pid_t ret; CALL_RETRY(ret,waitpid(tpid, &wexit, 0))
        if (ret != tpid)
            return -1;
        if (!WIFEXITED(wexit))
            return -1;
        status = WEXITSTATUS(wexit);
    }
    signal(SIGHUP, old_handler[SIGHUP]);
    signal(SIGINT, old_handler[SIGINT]);
    signal(SIGQUIT, old_handler[SIGQUIT]);
    signal(SIGILL, old_handler[SIGILL]);
    signal(SIGTRAP, old_handler[SIGTRAP]);
    signal(SIGABRT, old_handler[SIGABRT]);
    signal(SIGIOT, old_handler[SIGIOT]);
    signal(SIGFPE, old_handler[SIGFPE]);
    signal(SIGKILL, old_handler[SIGKILL]);
    signal(SIGUSR1, old_handler[SIGUSR1]);
    signal(SIGSEGV, old_handler[SIGSEGV]);
    signal(SIGUSR2, old_handler[SIGUSR2]);
    signal(SIGPIPE, old_handler[SIGPIPE]);
    signal(SIGTERM, old_handler[SIGTERM]);
#ifdef SIGSTKFLT
    signal(SIGSTKFLT, old_handler[SIGSTKFLT]);
#endif
    signal(SIGCHLD, old_handler[SIGCHLD]);
    signal(SIGCONT, old_handler[SIGCONT]);
    signal(SIGSTOP, old_handler[SIGSTOP]);
    signal(SIGTSTP, old_handler[SIGTSTP]);
    signal(SIGTTIN, old_handler[SIGTTIN]);
    signal(SIGTTOU, old_handler[SIGTTOU]);
    return status;
}

bool System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]
#ifdef __linux__
    , uid_t suid
#endif
) {
    System::tpid = fork();
    if (tpid < 0)
        return false;
    if (tpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        signal(SIGTRAP, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGIOT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGUSR1, SIG_DFL);
        signal(SIGSEGV, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
#ifdef SIGSTKFLT
        signal(SIGSTKFLT, SIG_DFL);
#endif
        signal(SIGCHLD, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGSTOP, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        
        char* args_c[256]{nullptr};
        args_c[0] = (char*)path;
        for (int i = 1; i < 256; i++)
            if (args[i-1] == nullptr) break;
            else args_c[i] = (char*)args[i-1];

    #ifdef __linux__
        if (suid) { int res = setuid(suid); }
    #endif

        execv(path, args_c);
        exit(127);
    }
    return true;
}

nstring System::ToNativePath(nstring path) {
    return path;
}

#ifdef __APPLE__
    nstring System::GetSelf(void) {
        char buf[PATH_MAX] = {'\0'};
        uint32_t bufsize = PATH_MAX;
        ssize_t size = _NSGetExecutablePath(buf, &bufsize);
        if (size < 0) exit(0xC3);
        return buf;
    }
#else
    nstring System::GetSelf(void) {
        char buf[PATH_MAX] = {'\0'};
        ssize_t size = readlink("/proc/self/exe", buf, PATH_MAX);
        if (size < 0) exit(0xC3);
        return buf;
    }
#endif

    nstring System::GetRoot(void) {
        if (!self.size()) System::self = System::GetSelf();
        char edit[PATH_MAX];
        auto size = System::self.size();
        while (System::self[--size] != '/')
        for (size_t i = 0; i <= size; i++) edit[i] = System::self[i];
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        nstring out = nstring(edit);
        return out;
    }

#endif

nstring System::GetRootDir(void) {
    return System::root;
}

uniconv::nstring System::GetSelfPath(void) {
    return System::self;
}


nint System::AllocateMemory(size_t bytes) {
    nint out = malloc(bytes);
#ifdef _DEBUG
    __save$ALLOCATIONS(out,bytes);
#endif
    return out;
}

void System::FreeMemory(nint pointer) {
#ifdef _DEBUG
    __free$ALLOCATIONS(pointer);
#endif
    free(pointer);
}

nint System::MovePointer(nint pointer, signed int bytes) {
    return (int8_t*)(pointer) + bytes;
}
