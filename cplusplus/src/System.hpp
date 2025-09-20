#pragma once

#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>

#include <control_heap.h>
#include <unordered_map>
#ifdef _WIN32
    #include <windows.h>
    #include <shlwapi.h>
    #include <windows/thread_safe/queue>
    __if_not_exists(quick_exit) {
        #include <quick_exit.h>
    }
#else
#ifdef INVALID_HANDLE_VALUE
    #undef INVALID_HANDLE_VALUE
    #undef ERROR_ACCESS_DENIED
#endif
    #define INVALID_HANDLE_VALUE ((int)-1)
    #define ERROR_ACCESS_DENIED EACCES
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <fcntl.h>
#ifdef _LINUX_WAIT_H
    #undef P_ALL
    #undef P_PID
    #undef P_PGID
    #undef P_PIDFD
#endif
    #include <sys/wait.h>
#ifdef __APPLE__
    #include <signal.h>
    #include <mach-o/dyld.h>
    #ifndef quick_exit
        #include <quick_exit.h>
    #endif
#elif __linux__
    #include <linux/limits.h>
#elif __CYGWIN__
    #include <process.h>
    #include <sys/cygwin.h> 

    #include <windows.h>
    #include <shlwapi.h>
#endif
#endif

typedef void* nint;

namespace cpp {
    #ifdef _WIN32
        typedef HANDLE fd_t;
    #else
        typedef int fd_t;
    #endif

    typedef struct rw_pipe {
        fd_t read;
        fd_t write;
        rw_pipe(void) : read(0),write(0) {}
        rw_pipe(fd_t r, fd_t w) : read(r), write(w) {}
    } rw_pipe_t;

    struct message_head {
        size_t msg_size;
    };

    typedef struct message {
        message_head head;
    #ifdef _WIN32
        wchar_t* msg;
    #else
        char* msg;
    #endif
    } message_t;

    class System {
    private:
        static uniconv::nstring GetRoot(void);
        static uniconv::nstring GetSelf(void);
        static uniconv::nstring self;
        static uniconv::nstring root;
#if !defined(_WIN32)
        static pid_t tpid;
        static void SendSignal(int signal);
        static std::unordered_map<fd_t, std::string> pipes; // pipe handle -> { pipe path }
#else
        static std::unordered_map<fd_t, std::pair<std::pair<HANDLE, tsqueue<std::wstring>>, std::wstring>> pipes; // pipe handle -> { { pipe thread , write queue } , symlink path }
#endif
    public:
        static uniconv::nstring GetRootDir(void);
        static uniconv::nstring GetSelfPath(void);
        static uniconv::nstring ToNativePath(uniconv::nstring path);

        static nint AllocateMemory(size_t bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        static int MakeDirectory(uniconv::utfcstr path);
        static int ClearDirectory(uniconv::utfcstr path);
        static int DeleteDirectory(uniconv::utfcstr path);
        static int RemoveFile(uniconv::utfcstr path);
        static bool IsFile(uniconv::utfcstr path);
        static bool IsDirectory(uniconv::utfcstr path);
        static bool DoesPathExist(uniconv::utfcstr path);
    #ifdef __CYGWIN__
        static uniconv::nstring WindowsPathToCygwin(std::wstring path);
        static std::wstring CygwinPathToWindows(uniconv::nstring path);
        static std::string CygwinPathToWindowsUtf8(uniconv::nstring path);
        static int RunProgram0(const char* cpath, const wchar_t* args, ...); // mintty fix
    #endif

        static fd_t CreatePipe(uniconv::utfcstr subpath); // the cre
        static fd_t OpenPipe(uniconv::utfcstr subpath); // read == false -> write
        static void ClosePipe(fd_t pipe);
        static void ClosePipe(rw_pipe_t pipe) { ClosePipe(pipe.read); ClosePipe(pipe.write); }
        // Non-blocking
        static int SendMessagePipe(fd_t pipe, uniconv::nstring msg);
        static int SendMessagePipe(rw_pipe_t pipe, uniconv::nstring msg) { return SendMessagePipe(pipe.write, msg); }
        // Blocking
        static uniconv::nstring ReadMessagePipe(fd_t pipe);
        static uniconv::nstring ReadMessagePipe(rw_pipe_t pipe) { return ReadMessagePipe(pipe.read); }

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, const T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, const T value);
        
        static int Shell(uniconv::utfcstr arg);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]
    #ifdef __linux__
        , uid_t suid = 0
    #endif
        );
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr const args[]);
        
        static bool ShellAsync(uniconv::utfcstr arg);  
        static bool RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static bool RunProgramAsyncS(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static bool RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]
    #ifdef __linux__
        , uid_t suid = 0
    #endif
        );
        static bool RunProgramAsyncS(uniconv::utfcstr file, uniconv::utfcstr const args[]);
    #ifdef _WIN32
        static int ShellC(uniconv::utfcstr arg);
        static int RunProgramC(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static int RunProgramSC(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static int RunProgramC(uniconv::utfcstr path, uniconv::utfcstr const args[]);
        static int RunProgramSC(uniconv::utfcstr file, uniconv::utfcstr const args[]);
        
        static bool ShellAsyncC(uniconv::utfcstr arg);  
        static bool RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static bool RunProgramAsyncSC(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static bool RunProgramAsyncC(uniconv::utfcstr path, uniconv::utfcstr const args[]);
        static bool RunProgramAsyncSC(uniconv::utfcstr file, uniconv::utfcstr const args[]);

        static int RunProgram0(uniconv::utfcstr path, uniconv::utfcstr arg, ...);
    #endif
    };
}

#include "System.ipp"