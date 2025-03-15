#pragma once

#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>
#include <stdexcept>
#include <vector>
#include "defines.h"

#include "dllimport.hpp"

namespace cpp {
    class System {
    public:
        static std::u16string GetRootDir(void);
        static std::u16string GetSelfPath(void);
        static std::u16string ToNativePath(std::u16string path);

        static nint AllocateMemory(size_t bytes);
        static void FreeMemory(nint pointer);

        static nint MovePointer(nint pointer, signed int bytes);

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
    };
}