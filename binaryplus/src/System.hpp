#pragma once

#include <stdint.h>
#include <unicode_conversion.hpp>

#include "defines.h"

namespace cpp {
    class System {
    public:
        static std::wstring GetRootPath(void);
        static std::wstring ToNativePath(std::wstring path);

        static nint AllocateMemory(unsigned int bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
    };
}