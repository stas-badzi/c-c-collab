#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unicode_conversion.hpp>
#include <sstream>
#include <optional>
#include "defines.h"
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

extern int sub(int);

extern int Main(void);

// todo: move all Console.cpp into here

enum Color : unsigned char {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7,

    LIGHT_BLACK = 8,
    LIGHT_RED = 9,
    LIGHT_GREEN = 10,
    LIGHT_YELLOW = 11,
    LIGHT_BLUE = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_CYAN = 14,
    LIGHT_WHITE = 15,

    DEFAULT = 16,
};

namespace cpp {
    inline void ThrowMsg(const char16_t* msg) { cppimp::ThrowMsg(uniconv::U16StringToUnicode(msg)); }
    inline void Exit(int code) { cppimp::Exit(code); }
    inline void QuickExit(int code) { cppimp::QuickExit(code); }

    class Console {

    public:
        struct MouseStatus {
            bool primary;
            bool secondary;
            bool middle;
            std::pair<bool,bool> scroll; // (is scrolling),(up or down)[windows/linux/freebsd - scroll up == move scroll whell (fingers on touchbad) down; down == move up | macos scroll like tablet/phone]
            unsigned int x;
            unsigned int y; 
            MouseStatus(void) {
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
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], size_t size, const uint8_t backgrounds[], const uint8_t foregrounds[]);
            // pass the size
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], size_t size);
            // last one has to be empty for the function to finish
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], const uint8_t backgrounds[], const uint8_t foregrounds[]);
            // \n is treated as the end of a row and should have no associated backgroud and foreground or the colors will shift to the right 
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters, const uint8_t backgrounds[], const uint8_t foregrounds[]);
            // last one has to be empty for the function to finish
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[]);
            // \n is treated as the end of a row
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters);

            char16_t character(void);
            void character(char16_t val);

            uint8_t foreground(void);
            void foreground(uint8_t val);

            uint8_t background(void);
            void background(uint8_t val);

            void ReverseColors(void);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(void* ptr, bool direct = false);
            Symbol(char16_t character, uint8_t foreground = 16, uint8_t background = 16);
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
        static char16_t** GetArgV(void);
        static std::optional<std::pair<int,std::u16string>> PopupWindow(int type, int argc, const char16_t* argv[], const char16_t title[] = nullptr) {
            uniconv::unichar** args = (uniconv::unichar**)cppimp::System_AllocateMemory(sizeof(uniconv::unichar*)*argc);
            for (int i = 0; i < argc; i++)
                args[i] = uniconv::U16StringToUnicode(argv[i]);
            uniconv::unichar* utitle = title ? uniconv::U16StringToUnicode(title) : nullptr;
            auto ret = cppimp::Console_PopupWindow(type, argc, args, utitle);
            if (ret.val) return std::pair<int,std::u16string>(ret.code,uniconv::UnicodeToU16String(ret.result));
            return std::nullopt;
        }
        static std::optional<std::pair<stsb::promise<std::optional<std::pair<int, std::u16string>>>,rw_pipe_t>> PopupWindowAsync(int type, int argc, const char16_t* argv[], const char16_t title[]) { return ::Console_PopupWindowAsync(type, argc, argv,title); } 
        static std::optional<std::u16string> GetParentMessage(void) { auto ret = cppimp::Console_GetParentMessage(); if (ret.is_val) return uniconv::UnicodeToU16String(ret.val); return std::nullopt; }
        static bool SendParentMessage(std::u16string message) { return cppimp::Console_SendParentMessage(uniconv::U16StringToUnicode(message)); }
        static std::optional<std::u16string> GetChildMessage(rw_pipe_t pipe) { auto ret = cppimp::Console_GetChildMessage(pipe); if (ret.is_val) return uniconv::UnicodeToU16String(ret.val); return std::nullopt; }
        static bool SendChildMessage(rw_pipe_t pipe, std::u16string message) { return cppimp::Console_SendChildMessage(pipe,uniconv::U16StringToUnicode(message)); }
        static void Sleep(double seconds = 1.0, bool sleep_input_thread = false) { return cppimp::Console_Sleep(seconds, sleep_input_thread); }
        static void ClearScreenBuffer(void) { return cppimp::Console_ClearScreenBuffer(); }
        static void FillScreen(const std::vector<std::vector<Symbol> >& symbols);
        static void HandleMouseAndFocus(void);
        static void DontHandleKeyboard(void) { return cppimp::Console_DontHandleKeyboard(); }
        static void ResetKeyboard(void) { return cppimp::Console_ResetKeyboard(); }
        static bool IsFocused(void);
        static struct MouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID
        static bool IsMouseButtonDown(uint8_t button) { return cppimp::Console_IsMouseButtonDown(button); }
        static void HandleOutput(void) { return cppimp::Console_HandleOutput(); }
        static void Update(void) { return cppimp::Console_Update(); }
        static void SetResult(std::u16string result) { return cppimp::Console_SetResult(uniconv::U16StringToUnicode(result)); }
        static void MoveCursor(int x, int y) { return cppimp::Console_MoveCursor(x,y); }
        static void ShowCursor(void) { return cppimp::Console_ShowCursor(); }
        static void HideCursor(void) { return cppimp::Console_HideCursor(); }
        static void SetCursorSize(uint8_t size) { return cppimp::Console_SetCursorSize(size); }
        static void SetTitle(std::u16string title) { return cppimp::Console_SetTitle(uniconv::U16StringToUnicode(title)); }
    };
    inline void flush(void) { Console::HandleOutput(); }
#if defined(_WIN32)
    extern __declspec(dllimport) std::basic_istream<wchar_t>& win;
    extern __declspec(dllimport) std::basic_ostream<wchar_t>& wout;
#else
    extern std::basic_istream<wchar_t>& win;
    extern std::basic_ostream<wchar_t>& wout;
#endif
}