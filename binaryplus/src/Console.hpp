#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unicode_conversion.hpp>
#include "ConsoleMouseStatus.hpp"
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
            Symbol(void* ptr, bool direct = false);
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
        static void Fin(void);
        static int HandleKeyboard(void);
        static bool IsKeyDown(int key);
        static bool IsKeyToggled(int key);
        static int KeyPressed(void);
        static int KeyReleased(void);
        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);
        static void Sleep(double seconds = 1.0);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol> > symbols);
        static void HandleMouseAndFocus(void);
        static bool IsFocused(void);
        static struct ConsoleMouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID
    };
}