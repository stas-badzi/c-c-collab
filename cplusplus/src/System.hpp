#pragma once

#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>

#include <control_heap.h>

#ifdef _WIN32
    #include <windows.h>
    #include <shlwapi.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/stat.h>
#ifndef _LINUX_WAIT_H
    #include <sys/wait.h>
#endif
#ifdef __APPLE__
    #include <signal.h>
    typedef void (*sighandler_t)(int);
#endif
#endif

#define nint void*

namespace cpp {
    class System {
    private:
        static uniconv::utfstr GetRoot(void);
        static uniconv::utfstr root;
        static uniconv::utfstr self;
#if !defined(_WIN32) && !defined(__CYGWIN__)
        static pid_t tpid;
        static void SendSignal(int signal);
#endif
    public:
        static uniconv::utfstr GetRootDir(void);
        static uniconv::utfstr GetSelfPath(void);
        static uniconv::utfstr ToNativePath(uniconv::utfstr path);

        static nint AllocateMemory(size_t bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        static int MakeDirectory(uniconv::utfcstr path);

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
        
        static int Shell(uniconv::utfcstr arg);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]);
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr const args[]);
        
        static bool ShellAsync(uniconv::utfcstr arg);  
        static bool RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static bool RunProgramAsyncS(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static bool RunProgramAsync(uniconv::utfcstr path, uniconv::utfcstr const args[]);
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
    #endif
    };
}

#include "System.ipp"