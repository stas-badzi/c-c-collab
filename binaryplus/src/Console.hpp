#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unicode_conversion.hpp>
#ifdef _WIN32
    #include <windows.h>
#endif

namespace cpp {
    class Console {

    public:
        struct Symbol {

            uniconv::utfchar character(void);
            void character(uniconv::utfchar val);

            uint8_t foreground(void);
            void foreground(uint8_t val);

            uint8_t background(void);
            void background(uint8_t val);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(uniconv::utfchar character, uint8_t foreground = 7, uint8_t background = 0);
            ~Symbol();

            Symbol operator=(const Symbol &src);

            void* Get();

            #ifdef _WIN32
                Symbol(uint8_t attribute);
                uint8_t GetAttribute(void);
                void SetAttribute(uint8_t attribute);
            #endif

        private:
            void* symbol;

        };
        static void Init(void);
        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol> > symbols);
    };
}