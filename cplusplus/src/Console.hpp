#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <unicode_conversion.hpp>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <iostream>
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

namespace cpp {
    class Console {
    private:
        static bool initialised;
        #ifdef _WIN32
            static HANDLE h_console;
        #endif
    public:
        static void Init(void);
        struct Symbol {
            uniconv::utfchar character;
            uint8_t foreground; // 0-16
            uint8_t background; // 0-16

            Symbol(void);

            Symbol(uniconv::utfchar character, uint8_t foreground = 7, uint8_t background = 0);

            Symbol(uint8_t attribute);

            ~Symbol(void);

            Symbol & operator=(const Symbol &src);

            #ifdef _WIN32
                uint8_t GetAttribute(void);
                void SetAttribute(uint8_t attribute);
            #endif

        };
        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol> > symbols);

    };
} // namespace cpp