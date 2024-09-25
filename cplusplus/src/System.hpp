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
#endif

#define nint void*

namespace cpp {
    class System {
    private:
        static uniconv::utfstr GetRoot(void);
        static uniconv::utfstr root;
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
    };

#include "System.ipp"
}