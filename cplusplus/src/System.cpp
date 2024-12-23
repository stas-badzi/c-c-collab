#include "System.hpp"
#include <stdarg.h>

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
#else


int cpp::System::Shell(uniconv::utfcstr command) {
    return System::RunProgram("/bin/sh", "-c", command, nullptr); 
}

#define CALL_RETRY(retvar, expression) do { \
    retvar = (expression); \
} while (retvar == -1 && errno == EINTR);

pid_t cpp::System::tpid = 0;

void cpp::System::SendSignal(int signal) {
    kill(tpid, signal);
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
            const char* arg = va_arg(args, const char*);
            if (arg == nullptr) break;
            args_c[i] = (char*)arg;
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
            const char* arg = va_arg(args, const char*);
            if (arg == nullptr) break;
            args_c[i] = (char*)arg;
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
