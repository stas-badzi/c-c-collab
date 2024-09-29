#pragma once

#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>
#include <stdexcept>
#include "defines.h"

#include "dllimport.hpp"

namespace cpp {
    class System {
    public:
        static std::wstring GetRootPath(void);
        static std::wstring ToNativePath(std::wstring path);

        static nint AllocateMemory(size_t bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        static std::vector<typename T> MakeList(size_t count, T elem);
        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
    };
}