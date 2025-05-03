#pragma once

#include <stdint.h>
#include <unicode_conversion.hpp>
#include <typeinfo>
#include <stdexcept>
#include <vector>
#include <unicode_conversion.hpp>
#include "dllimport.hpp"
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

        static int MakeDirectory(std::u16string path) { return cppimp::System_MakeDirectory(uniconv::U16StringToUnicode(path)); }
        static int ClearDirectory(std::u16string path) { return cppimp::System_ClearDirectory(uniconv::U16StringToUnicode(path)); }
        static int DeleteDirectory(std::u16string path) { return cppimp::System_DeleteDirectory(uniconv::U16StringToUnicode(path)); }
        static int RemoveFile(std::u16string path) { return cppimp::System_RemoveFile(uniconv::U16StringToUnicode(path)); }
        static bool IsFile(std::u16string path) { return cppimp::System_IsFile(uniconv::U16StringToUnicode(path)); }
        static bool IsDirectory(std::u16string path) { return cppimp::System_IsDirectory(uniconv::U16StringToUnicode(path)); }
        static bool DoesPathExist(std::u16string path) { return cppimp::System_DoesPathExist(uniconv::U16StringToUnicode(path)); }

        template<typename T> static T ReadPointer(nint pointer);
        template<typename T> static T ReadPointer(nint pointer, int offset);
        template<typename T> static void WritePointer(nint& pointer, T value);
        template<typename T> static void WritePointer(nint& pointer, int offset, T value);
    };
}