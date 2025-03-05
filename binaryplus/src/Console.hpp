#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unicode_conversion.hpp>
#include <sstream>
#include "dllimport.hpp"
#ifdef _WIN32
    //#include <windows.h>
    #include <windows/key.hpp>
    //#include <iostream>
#elif __linux__
    #include <linux/key.hpp>
#elif __APPLE__
    #include <apple/key.hpp>
#else
#endif

#define MOUSE_BUTTON_1  1
#define MOUSE_BUTTON_2  2
#define MOUSE_BUTTON_3  3
#define MOUSE_BUTTON_4  4
#define MOUSE_BUTTON_5  5
#define MOUSE_BUTTON_6  6
#define MOUSE_BUTTON_7  7
#define MOUSE_BUTTON_8  8
#define MOUSE_BUTTON_9  9
#define MOUSE_BUTTON_10 10
#define MOUSE_BUTTON_11 11
#define MOUSE_BUTTON_PRIMARY MOUSE_BUTTON_1 // 1
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_2 // 2
#define MOUSE_BUTTON_SECONDARY MOUSE_BUTTON_3 // 3
#define MOUSE_SCROLL_UP MOUSE_BUTTON_4 // 4
#define MOUSE_SCROLL_DOWN MOUSE_BUTTON_5 // 5
#define MOUSE_MODIFIER_SHIFT 0b10000 // 2^4
#define MOUSE_MODIFIER_META 0b100000 //2^5
#define MOUSE_MODIFIER_CONTROL 0b1000000 // 2^6
#define MOUSE_MODIFIER_ALT MOUSE_MODIFIER_META // 2^6

int sub(int);

// todo: move all Console.cpp into here

namespace cpp {
    inline void ThrowMsg(const wchar_t* msg) { cppimp::ThrowMsg(uniconv::Utf8StringToUnicode(uniconv::WStringToNative(msg).c_str())); }

    class Console {

    public:
        struct MouseStatus {
            bool primary;
            bool secondary;
            bool middle;
            std::pair<bool,bool> scroll; // (is scrolling),(up or down)[windows/linux/freebsd - scroll up == move scroll whell (fingers on touchbad) down; down == move up | macos scroll like tablet/phone]
            unsigned int x;
            unsigned int y; 
            inline MouseStatus(void) {
                this->primary = false;
                this->middle = false;
                this->secondary = false;
                this->scroll = std::pair<bool,bool>(false,false);
                this->x = 0;
                this->y = 0;
            }
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

            void ReverseColors(void);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(void* ptr, bool direct = false);
            Symbol(wchar_t character, uint8_t foreground = 16, uint8_t background = 16);
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
        static void HandleKeyboard(void);
        static bool IsKeyDown(enum Key::Enum key);
        static struct ToggledKeys KeysToggled(void);
        static enum Key::Enum KeyPressed(void);
        static enum Key::Enum KeyReleased(void);
        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);
        static int32_t GetArgC(void);
        static wchar_t** GetArgV(void);
        static int PopupWindow(int type, int argc, wchar_t* argv[]);
        static void Sleep(double seconds = 1.0);
        static void FillScreen(std::vector<std::vector<Symbol> > symbols);
        static void HandleMouseAndFocus(void);
        static bool IsFocused(void);
        static struct MouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID
        static inline void Update(void) { return cppimp::Console_Update(); }
        static inline void MoveCursor(int x, int y) { return cppimp::Console_MoveCursor(x,y); }
        static inline void ShowCursor(void) { return cppimp::Console_ShowCursor(); }
        static inline void HideCursor(void) { return cppimp::Console_HideCursor(); }
        static inline void SetCursorSize(uint8_t size) { return cppimp::Console_SetCursorSize(size); }
        static inline void SetTitle(std::wstring title) { return cppimp::Console_SetTitle(uniconv::Utf8StringToUnicode(uniconv::WStringToNative(title).c_str())); }
    };
#if defined(_WIN32) || defined(__CYGWIN__)
    extern __declspec(dllimport) std::wistream& gin;
    extern __declspec(dllimport) std::wostream& gout;
#else
    extern std::wistream& gin;
    extern std::ostream& gout;
#endif
}