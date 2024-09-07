#pragma once

#include <stdlib.h>
#include <string>
#include <stdint.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#define nint void*

namespace cpp {
    class System {
    private:
        static std::string GetRoot(void);
        static std::string root;
    public:
        static std::string GetRootPath(void);
        static std::string ToNativePath(std::string path);

        void* AllocateMemory(unsigned int bytes);
        void FreeMemory(void* pointer);

        void* MovePointer(void* pointer, signed int bytes);

        template<typename T> T ReadPointer(void* pointer);
        template<typename T> T ReadPointer(void* pointer, int offset);
        template<typename T> void WritePointer(void*& pointer, T value);
        template<typename T> void WritePointer(void*& pointer, int offset, T value);
    };
}