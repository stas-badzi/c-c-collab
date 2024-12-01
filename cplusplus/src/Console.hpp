#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <utility>
#include <clang_constexpr.h>

#ifdef __APPLE__
#ifndef _GLIBCXX_HAVE_AT_QUICK_EXIT
    #define _GLIBCXX_HAVE_QUICK_EXIT
#endif
#endif
#include <cstdlib>
#include <cstdint>
#include <signal.h>
#include <chrono>
#include <math.h>
#include <exception>

#include <unicode_conversion.hpp>

#ifdef _WIN32
    #include <windows.h>
    #include <windows/key.hpp>
#ifndef _MSVC
    #include <windows/quick_exit.h>
#endif
#else
    #include <signal.h>
    #include <stdio.h>
    #include <limits>
    #include <climits>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <deque>
    #include <string>
    #include <string.h>
#ifdef __linux__
    #include <linux/getfd.h>
    #include <linux/kd.h>
    #include <linux/keyboard.h>
    #include <linux/key.hpp>
    #include <linux/ledctrl.h>
#elif __APPLE__
    #include <apple/key.hpp>
#elif __CYGWIN__
    #include <windows.h>
    #include <windows/key.hpp>
#else
#endif
#endif
    #define KEYBOARD_MAX 256

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

namespace cpp {
    class Console {
    public:
        struct MouseStatus {
            bool primary; // is down
            bool secondary; // is down
            bool middle; // is down
            std::pair<bool,bool> scroll; // (is scrolling),(up or down)[windows/linux/freebsd - scroll up == move scroll whell (fingers on touchbad) down; down == move up | macos scroll like tablet/phone]
            unsigned int x; // in console chracters
            unsigned int y; // in console chracters
            MouseStatus(void);
        };
        
    private:
        static bool initialised;
        static std::bitset<KEYBOARD_MAX> key_states;
        static int key_hit;
        static int key_released;
        static unsigned short double_click_max; // = 500;
        static std::bitset<5> mouse_buttons_down;
        static bool focused;
        static struct MouseStatus mouse_status;
        static uint8_t this_mouse_button;
        static uint8_t this_mouse_combo;
        static bool this_mouse_down;
        static uint8_t last_mouse_button;
        static uint8_t last_mouse_combo;
        static std::chrono::time_point<std::chrono::high_resolution_clock> last_click_time;
        static int argc;
        static uniconv::utfcstr* argv;
        static struct ToggledKeys keys_toggled;
        static bool emulator;
        #ifdef _WIN32
            //static std::vector<std::vector<COLORREF>> SaveScreen(void);
            //static std::pair<std::pair<uint16_t,uint16_t>,std::pair<uint16_t,uint16_t>> GetOffsetSymSize(int color1 = 3, int color2 = 9, int color3 = 1);
            
            //static std::pair<uint16_t,uint16_t> scr_offs;
            //static std::pair<uint16_t,uint16_t> sym_size;
            //static bool auto_size_updates;
            //static int16_t old_width;
            //static int16_t old_height;
            //static RECT old_rect;
            static uint8_t default_fcol;
            static uint8_t default_bcol;
            static HANDLE screen;
            static HANDLE fd;
            static HWND window;
            static HDC device;
            static DWORD old_console;
            static HANDLE old_buffer;
            static inline uint8_t GenerateAtrVal(uint8_t i1, uint8_t i2);
            //static std::pair<uint16_t,uint16_t> xyoffset;
            //static inline std::pair<uint16_t,uint16_t> GetXYCharOffset();
        #else
            static struct termios old_termios;
            static struct winsize window_size;
            static char buf[256]; static int8_t buf_it;
        #ifdef __linux__
            static inline char GetChar(void);
            static struct termios old_fdterm;
            static int old_kbdmode;
            static int fd;
            static Key::Enum key_chart[MAX_NR_KEYMAPS][KEYBOARD_MAX];
        #endif
        #endif
    public:
        static void Init(void);
        static void Fin(void);

        struct Symbol {
            uniconv::utfchar character;
            uint8_t foreground; // 0-16
            uint8_t background; // 0-16

            Symbol(void);

            Symbol(const Symbol& sym);

            Symbol(uniconv::utfchar character, uint8_t foreground = 16, uint8_t background = 16);

            Symbol(uint8_t attribute);

            ~Symbol(void);

            void ReverseColors(void);

            Symbol & operator=(const Symbol &src);

            #ifdef _WIN32
                uint8_t GetAttribute(void);
                void SetAttribute(uint8_t attribute);
            #endif

        };
        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);

        static int32_t GetArgC(void);
        static uniconv::utfcstr* GetArgV(void);

        static void FillScreen(std::vector<std::vector<Symbol> > symbols);
        
        static void HandleKeyboard(void);
        static bool IsKeyDown(enum Key::Enum key);
        static struct ToggledKeys KeysToggled(void);
        static enum Key::Enum KeyPressed(void);
        static enum Key::Enum KeyReleased(void);
        static enum Key::Enum KeyTyped(void);

        static void HandleMouseAndFocus(void);
        static bool IsFocused(void);
        static struct MouseStatus GetMouseStatus(void);
        static std::pair<uint8_t,uint8_t> MouseButtonClicked(void); // returns button ID and whitch consecutive click was it
        static uint8_t MouseButtonReleased(void); // returns button ID

        static void Sleep(double seconds = 1);

        static void SetDoubleClickMaxWait(unsigned short milliseconds);
        static unsigned short GetDoubleClickMaxWait(void);
    };
} // namespace cpp

void SysSleep(int microseconds);