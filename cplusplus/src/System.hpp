#pragma once

#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>

#include <control_heap.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#ifndef _LINUX_WAIT_H
    #include <sys/wait.h>
#endif
#endif

#define nint void*

namespace cpp {
    class System {
    private:
        static uniconv::utfstr GetRoot(void);
        static uniconv::utfstr root;
#ifdef __UNIX__
        static pid_t tpid;
        static void SendSignal(int signal);
#endif
    public:
        static uniconv::utfstr GetRootPath(void);
        static uniconv::utfstr ToNativePath(uniconv::utfstr path);

        static nint AllocateMemory(size_t bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
        
        static int Shell(uniconv::utfcstr arg);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr args, ...);
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr args, ...);
        static int RunProgram(uniconv::utfcstr path, uniconv::utfcstr const args[]);
        static int RunProgramS(uniconv::utfcstr file, uniconv::utfcstr const args[]);
    };
}

#include "System.ipp"