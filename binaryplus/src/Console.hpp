#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unicode_conversion.hpp>
#include <sstream>
#include <optional>
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
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], size_t size, uint8_t backgrounds[], uint8_t foregrounds[]);
            // pass the size
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], size_t size);
            // last one has to be empty for the function to finish
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters[], uint8_t backgrounds[], uint8_t foregrounds[]);
            // \n is treated as the end of a row and should have no associated backgroud and foreground or the colors will shift to the right 
            static std::vector<std::vector<Symbol> > CreateTexture(std::u16string characters, uint8_t backgrounds[], uint8_t foregrounds[]);
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
        static inline std::optional<std::pair<int,std::u16string>> PopupWindow(int type, int argc, const char16_t* argv[]) {
            uniconv::unichar** args = (uniconv::unichar**)cppimp::System_AllocateMemory(sizeof(uniconv::unichar*)*argc);
            for (int i = 0; i < argc; i++)
                args[i] = uniconv::U16StringToUnicode(argv[i]);
            auto ret = cppimp::Console_PopupWindow(type, argc, args);
            if (ret.val) return std::pair<int,std::u16string>(ret.code,uniconv::UnicodeToU16String(ret.result));
            return std::nullopt;
        }
        static inline std::optional<stsb::promise<std::optional<std::pair<int, std::u16string>>>> PopupWindowAsync(int type, int argc, const char16_t* argv[]) { return cppimp::Console_PopupWindowAsync(type, argc, argv); }
        static void Sleep(double seconds = 1.0);
        static void FillScreen(std::vector<std::vector<Symbol> > symbols);
        static void HandleMouseAndFocus(void);
        static inline void DontHandleKeyboard(void) { return cppimp::Console_DontHandleKeyboard(); }
        static inline void ResetKeyboard(void) { return cppimp::Console_ResetKeyboard(); }
        static bool IsFocused(void);
        static struct MouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID
        static inline void Update(void) { return cppimp::Console_Update(); }
        static inline void SetResult(std::u16string result) { return cppimp::Console_SetResult(uniconv::U16StringToUnicode(result)); }
        static inline void MoveCursor(int x, int y) { return cppimp::Console_MoveCursor(x,y); }
        static inline void ShowCursor(void) { return cppimp::Console_ShowCursor(); }
        static inline void HideCursor(void) { return cppimp::Console_HideCursor(); }
        static inline void SetCursorSize(uint8_t size) { return cppimp::Console_SetCursorSize(size); }
        static inline void SetTitle(std::u16string title) { return cppimp::Console_SetTitle(uniconv::U16StringToUnicode(title)); }
    };
#if defined(_WIN32) || defined(__CYGWIN__)
    extern __declspec(dllimport) std::wistream& gin;
    extern __declspec(dllimport) std::wostream& gout;
#else
    extern std::wistream& gin;
    extern std::ostream& gout;
#endif
}