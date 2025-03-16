#include "System.hpp"

#include "Console.hpp"
#include <stdarg.h>

#include <iostream>
#include <cstring>
#include <wchar.h>

using namespace cpp;
using namespace std;
using namespace uniconv;

utfstr System::root = System::GetRoot();
utfstr System::self = utfstr();

#ifdef _WIN32
#ifdef _MSC_VER
    #define PATH_MAX MAX_PATH
#endif
    utfstr System::GetRoot(void) {
        wchar_t buf[PATH_MAX];
        DWORD size = GetModuleFileName(NULL, buf, PATH_MAX);
        wchar_t* edit = buf;
        System::self = edit;
        while (edit[--size] != '\\') buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        utfstr out = (utfstr)(edit);
        return out;
    }

    utfstr cpp::System::ToNativePath(utfstr path) {
        for (int i = 0; path[i] != L'\0'; i++) if (path[i] == L'/') path[i] = L'\\';
        return path;
    }

    int cpp::System::MakeDirectory(utfcstr path) {
        Console::out << L"MakeDirectory: " << path << L'\n';  
        SECURITY_ATTRIBUTES sec_atrs{};
        sec_atrs.nLength = sizeof(SECURITY_ATTRIBUTES);
        sec_atrs.lpSecurityDescriptor = nullptr;
        sec_atrs.bInheritHandle = false;
        if (CreateDirectory(path, &sec_atrs)) return 0;
        return GetLastError(); 
    }

    int cpp::System::ClearDirectory(uniconv::utfcstr path) {
        wstring folderPath = path;
        if (folderPath.back() != L'\\') folderPath.push_back(L'\\');
        folderPath.push_back(L'*');
        WIN32_FIND_DATA info;
        HANDLE FindHndle = FindFirstFileEx(folderPath.c_str(), FindExInfoBasic, &info, FindExSearchNameMatch, NULL, 0);
        do {
            wstring fileFound = path;
            if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(info.cFileName, L".") == 0 || wcscmp(info.cFileName, L"..") == 0) continue;
                if (fileFound.back() != L'\\') fileFound.push_back(L'\\');
                fileFound.append(info.cFileName);
                ClearDirectory(fileFound.c_str());
                DeleteDirectory(fileFound.c_str());
                continue;
            }
            if (fileFound.back() != L'\\') fileFound.push_back(L'\\');
            fileFound.append(info.cFileName);
            if (RemoveFile(fileFound.c_str())) exit(0xC3);
        } while(FindNextFile(FindHndle, &info));
        return !FindClose(FindHndle);
    }

    int cpp::System::DeleteDirectory(uniconv::utfcstr path) {
        if (RemoveDirectory(path)) return 0;
        return GetLastError();
    }

    int cpp::System::RemoveFile(uniconv::utfcstr path) {
        if (DeleteFile(path)) return 0;
        return GetLastError();
    }

    bool cpp::System::IsFile(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool cpp::System::IsDirectory(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool cpp::System::DoesPathExist(uniconv::utfcstr path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES);
    }

    int cpp::System::Shell(uniconv::utfcstr arg) {
        return System::RunProgram(L"C:\\Windows\\System32\\cmd.exe", L"/c", arg);
    }

    bool cpp::System::ShellAsync(uniconv::utfcstr arg) {
        return System::RunProgramAsync(L"C:\\Windows\\System32\\cmd.exe", L"/c", arg);
    }

    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramC
    int cpp::System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (arg == nullptr) goto noargs;
        no_args = false;
        va_start(args, arg);
        args_v += L" ";
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            args_v += L" ";
            args_v += argx;
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
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    int cpp::System::RunProgramC(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
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
            args_v += L"\" \"";
            args_v += argx;
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
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    
    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramAsyncC
    bool cpp::System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
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
            args_v += L"\" \"";
            args_v += argx;
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
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    bool cpp::System::RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
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
            args_v += L"\" \"";
            args_v += argx;
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

    int cpp::System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" ";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            args_v += L" ";
            args_v += argx;
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
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    int cpp::System::RunProgramC(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            args_v += L"\" \"";
            args_v += argx;
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
            return exitcode;
        } else {
            CloseHandle(pi.hProcess);
            cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
            exit(0x63);
        }
    }

    
    // instead or runnning conhost.exe, run cmd.exe or another program with RunProgramAsyncC
    bool cpp::System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            args_v += L"\" \"";
            args_v += argx;
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
        if (no_args)
            status = CreateProcess(path, nullptr, nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        else status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
        
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }

    bool cpp::System::RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
        wstring args_v = L'\"' + wstring(path) + L'\"';
        bool no_args = true;
        if (args[0] == nullptr) goto noargs;
        no_args = false;
        args_v += L" \"";
        args_v += args[0];
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = args[i-1];
            if (argx == nullptr) break;
            args_v += L"\" \"";
            args_v += argx;
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

int cpp::System::MakeDirectory(utfcstr path) {
    if (mkdir(path, 0777) == 0) return 0;
    return errno;
}

int cpp::System::DeleteDirectory(utfcstr path) {
    if (rmdir(path) == 0) return 0;
    return errno;
}

int cpp::System::RemoveFile(utfcstr path) {
    if (unlink(path) == 0) return 0;
    return errno;
}

int cpp::System::Shell(uniconv::utfcstr command) {
    return System::RunProgram("/bin/sh", "-c", command, nullptr); 
}

bool cpp::System::ShellAsync(uniconv::utfcstr command) {
    return System::RunProgramAsync("/bin/sh", "-c", command, nullptr);
}

#define CALL_RETRY(retvar, expression) do { \
    retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

pid_t cpp::System::tpid = 0;

void cpp::System::SendSignal(int signal) {
    kill(tpid, signal);
}

bool cpp::System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
    int status;
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



int cpp::System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
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

int cpp::System::RunProgramS(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
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

int cpp::System::RunProgramS(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
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

int cpp::System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
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

bool cpp::System::RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]) {
    int status;
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

utfstr System::ToNativePath(utfstr path) {
    return path;
}

#ifdef __APPLE__
    utfstr System::GetRoot(void) {
        char buf[PATH_MAX] = {'\0'};
        uint32_t bufsize = PATH_MAX;
        ssize_t size = _NSGetExecutablePath(buf, &bufsize);
        if (size < 0) exit(0xC3);
        else size = strlen(buf);
        char *edit = buf;
        System::self = edit;
        while (edit[--size] != '/')
            buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        utfstr out = (utfstr)(edit);
        return out;
    }
#else
    utfstr System::GetRoot(void) {
        char buf[PATH_MAX] = {'\0'};
        ssize_t size = readlink("/proc/self/exe", buf, PATH_MAX);
        if (size < 0) exit(0xC3);
        char *edit = buf;
        System::self = edit;
        while (edit[--size] != '/')
            buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        utfstr out = (utfstr)(edit);
        return out;
    }
#endif
#endif

utfstr System::GetRootDir(void) {
    return System::root;
}



uniconv::utfstr cpp::System::GetSelfPath(void) {
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
