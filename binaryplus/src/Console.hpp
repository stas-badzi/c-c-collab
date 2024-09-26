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
        struct MouseStatus {
            bool primary;
            bool secondary;
            bool middle;
            std::pair<bool,bool> scroll;
            unsigned int x;
            unsigned int y; 
            MouseStatus(void);
        };
        struct Symbol {
            // pass the size
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters[], size_t size, uint8_t backgrounds[], uint8_t foregrounds[]);
            // pass the size
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters[], size_t size);
            // last one has to be empty for the function to finish
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters[], uint8_t backgrounds[], uint8_t foregrounds[]);
            // \n is treated as the end of a row and should have no associated backgroud and foreground or the colors will shift to the right 
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters, uint8_t backgrounds[], uint8_t foregrounds[]);
            // last one has to be empty for the function to finish
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters[]);
            // \n is treated as the end of a row
            static std::vector<std::vector<Symbol> > CreateTexture(std::wstring characters);

            wchar_t character(void);
            void character(wchar_t val);

            uint8_t foreground(void);
            void foreground(uint8_t val);

            uint8_t background(void);
            void background(uint8_t val);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(void* ptr, bool direct = false);
            Symbol(uniconv::utfchar character, uint8_t foreground = 16, uint8_t background = 16);
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
        static void FillScreen(std::vector<std::vector<Symbol> > symbols);
        static void HandleMouseAndFocus(void);
        static bool IsFocused(void);
        static struct MouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID
    };
}