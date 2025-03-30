#include <string>
#include <stdarg.h>

#include <iostream>
#include <cstring>
#include <wchar.h>

#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <typeinfo>

#if defined(_WIN32) || defined(__CYGWIN__)
    #include <windows.h>
    #include <shlwapi.h>
    #define N(c) L##c
    #define sep L'\\'
    #define binary L".exe"
#else
    #define N(c) c
    #define sep '/'
    #include <limits.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/stat.h>
#ifndef _LINUX_WAIT_H
    #include <sys/wait.h>
#endif
#ifdef __APPLE__
    #include <signal.h>
    #include <mach-o/dyld.h>
    typedef void (*sighandler_t)(int);
    #define binary ".app"
#elif __linux__
    #include <linux/limits.h>
    #define binary ".bin"
#endif
#endif

using namespace std;

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef _MSC_VER
    #define PATH_MAX MAX_PATH
#endif
    wstring GetExeDir(void) {
        wchar_t buf[PATH_MAX];
        DWORD size = GetModuleFileName(NULL, buf, PATH_MAX);
        wchar_t* edit = buf;
        while (edit[--size] != '\\') buf[size] = '\0';
        edit[size+1] = '\0';
        wstring out = wstring(edit);
        return out;
    }
    int RunProgram(const wchar_t* path, int argc, char* argv[]) {
        PROCESS_INFORMATION pi = PROCESS_INFORMATION();
        STARTUPINFO si = STARTUPINFO();
        si.cb = sizeof(STARTUPINFO);
        int status;
        DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;
        status = CreateProcess(path, GetCommandLine(), nullptr, nullptr, false, dwCreationFlags, nullptr, nullptr, &si, &pi);
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

    
#else

#define CALL_RETRY(retvar, expression) do { \
    retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

pid_t tpid;
void SendSignal(int signal) { kill(tpid, signal); }

static int RunProgram(const char* path, int argc, char* argv[]) {
    int status;
    tpid = fork();
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
        
        char* args_c[ARG_MAX+1]{nullptr};
        for (int i = 0; i < argc; i++) {
            args_c[i] = argv[i];
        }
        execvp(path, args_c);
        exit(127);
    } else {
        old_handler[SIGHUP] = signal(SIGHUP, SendSignal);
        old_handler[SIGINT] = signal(SIGINT, SendSignal);
        old_handler[SIGQUIT] = signal(SIGQUIT, SendSignal);
        old_handler[SIGILL] = signal(SIGILL, SendSignal);
        old_handler[SIGTRAP] = signal(SIGTRAP, SendSignal);
        old_handler[SIGABRT] = signal(SIGABRT, SendSignal);
        old_handler[SIGIOT] = signal(SIGIOT, SendSignal);
        old_handler[SIGFPE] = signal(SIGFPE, SendSignal);
        old_handler[SIGKILL] = signal(SIGKILL, SendSignal);
        old_handler[SIGUSR1] = signal(SIGUSR1, SendSignal);
        old_handler[SIGSEGV] = signal(SIGSEGV, SendSignal);
        old_handler[SIGUSR2] = signal(SIGUSR2, SendSignal);
        old_handler[SIGPIPE] = signal(SIGPIPE, SendSignal);
        old_handler[SIGTERM] = signal(SIGTERM, SendSignal);
    #ifdef SIGSTKFLT
        old_handler[SIGSTKFLT] = signal(SIGSTKFLT, SendSignal);
    #endif
        old_handler[SIGCHLD] = signal(SIGCHLD, SendSignal);
        old_handler[SIGCONT] = signal(SIGCONT, SendSignal);
        old_handler[SIGSTOP] = signal(SIGSTOP, SendSignal);
        old_handler[SIGTSTP] = signal(SIGTSTP, SendSignal);
        old_handler[SIGTTIN] = signal(SIGTTIN, SendSignal);
        old_handler[SIGTTOU] = signal(SIGTTOU, SendSignal);
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

#ifdef __APPLE__
    static string GetExeDir(void) {
        char buf[PATH_MAX] = {'\0'};
        uint32_t bufsize = PATH_MAX;
        ssize_t size = _NSGetExecutablePath(buf, &bufsize);
        if (size < 0) exit(0xC3);
        else size = strlen(buf);
        char *edit = buf;
        while (edit[--size] != '/')
            buf[size] = '\0';
        edit[size+1] = '\0';
        string out = (string)(edit);
        return out;
    }
#else
    static string GetExeDir(void) {
        char buf[PATH_MAX] = {'\0'};
        ssize_t size = readlink("/proc/self/exe", buf, PATH_MAX);
        if (size < 0) exit(0xC3);
        char *edit = buf;
        while (edit[--size] != '/')
            buf[size] = '\0';
        edit[size+1] = '\0';
        string out = (string)(edit);
        return out;
    }
#endif
#endif


int main(int argc, char* argv[]) {
    auto root = GetExeDir() + N("bin");
    root.push_back(sep);
    root.append(N("cpp-factoryrush")).append(binary);
    int ret = RunProgram(root.c_str(), argc, argv);
    return ret;
}