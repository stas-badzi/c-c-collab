#include "System.hpp"
#include <stdarg.h>

#include <iostream>
#include <cstring>
#include <wchar.h>

using namespace cpp;
using namespace std;
using namespace uniconv;

utfstr System::root = System::GetRoot();

#ifdef _WIN32
    utfstr System::GetRoot(void) {
        wchar_t buf[255];
        DWORD size = GetModuleFileName(NULL, buf, 255);
        wchar_t* edit = buf;
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

    int cpp::System::Shell(uniconv::utfcstr arg) {
        return System::RunProgram(L"", arg);
    }

    int cpp::System::RunProgram(uniconv::utfcstr path, uniconv::utfcstr arg, ...) {
        va_list args;
        wstring args_v = path;
        if (arg == nullptr) goto noargs;
        va_start(args, arg);
        args_v += arg;
        for (int i = 2; i < 64; i++) {
            const wchar_t* argx = va_arg(args, const wchar_t*);
            if (argx == nullptr) break;
            args_v += argx; 
        }
        va_end(args);
    noargs:
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = NULL;
        si.hStdOutput = NULL;
        si.hStdError = NULL;
        si.wShowWindow = SW_NORMAL;
        si.lpDesktop = (wchar_t*)path;
        si.lpTitle = NULL;
        si.dwX = 0; // change later (maybe)
        si.dwY = 0; // same
        si.dwXSize = 0; // and same
        si.dwYSize = 0; // ---''---
        si.dwXCountChars = 0;
        si.dwYCountChars = 0;
        si.dwFillAttribute = 0;

        if (path == NULL) cerr << "path is null" << endl;
        else wcerr << L"path is " << path << endl;
        if (args_v.empty()) cerr << "args_v is empty" << endl;
        else wcerr << L"args_v is " << args_v << endl;
        int status = CreateProcess(path, (wchar_t*)args_v.c_str(), nullptr, nullptr, false, CREATE_PRESERVE_CODE_AUTHZ_LEVEL, nullptr, nullptr, &si, &pi);
        if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
        bool isdone = false;
        DWORD exitcode;
        while (!GetExitCodeProcess(pi.hProcess, &exitcode) && GetLastError() == STILL_ACTIVE) Sleep(1);
        return exitcode;
    }

#else


int cpp::System::Shell(uniconv::utfcstr command) {
    return System::RunProgram("/bin/sh", "-c", command, nullptr); 
}

int cpp::System::ShellAsync(uniconv::utfcstr command, bool& is_running) {
    return System::RunProgramAsync("/bin/sh", is_running, "-c", command, nullptr);
}

#define CALL_RETRY(retvar, expression) do { \
    retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

pid_t cpp::System::tpid = 0;

void cpp::System::SendSignal(int signal) {
    kill(tpid, signal);
}

struct thread_inout {
    void* args;
    void* ret;
};

struct __pid_bool {
    pid_t pid;
    bool& is_running;
};

void* WaitForProgram(void* pid_running_ret) {
    thread_inout inout = *(thread_inout*)pid_running_ret;
    int wexit, status;
    __pid_bool args = *(__pid_bool*)inout.args;
    pid_t pid = args.pid;
    bool& is_running = args.is_running;
    is_running = true;
    pid_t ret; CALL_RETRY(ret,waitpid((pid_t)pid, &wexit, 0))
    is_running = false;
    if (ret != (pid_t)pid)
        return (void*)-1;
    if (!WIFEXITED(wexit))
        return (void*)-1;
    status = WEXITSTATUS(wexit);
    *(int*)inout.ret = status;
    return 0;
}

int cpp::System::RunProgramAsync(uniconv::utfcstr path, bool& is_running, uniconv::utfcstr arg, ...) {
    is_running = true;
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
        return 0;
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

utfstr System::GetRoot(void)
{
    char buf[255] = {'\0'};
    ssize_t size = readlink("/proc/self/exe", buf, 255);
    char *edit = buf;
    while (edit[--size] != '/')
        buf[size] = '\0';
    edit[size + 1] = '.';
    edit[size + 2] = '.';
    edit[size + 3] = '\0';
    utfstr out = (utfstr)(edit);
    return out;
    }

    utfstr System::ToNativePath(utfstr path) {
        return path;
    }
#endif

    utfstr System::GetRootPath(void)
    {
        return System::root;
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
