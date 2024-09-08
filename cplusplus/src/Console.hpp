#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <cstdlib>
#include <cstdint>
#include <signal.h>
#include <chrono>
#include <math.h>
#include <exception>

#include <unicode_conversion.hpp>

#ifdef _WIN32
    #include <windows.h>
    #include <quick_exit.h>
#else
    #include <signal.h>
    #include <stdio.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <deque>
    #include <string>
#ifdef __linux__
    #include <getfd.h>
    #include <linux/kd.h>
#else
#endif
#endif

#define KEYBOARD_MAX 256

#define MOUSE_BUTTON_1 0b000000
#define MOUSE_BUTTON_2 0b000001
#define MOUSE_BUTTON_3 0b000010
#define MOUSE_BUTTON_4 0b000011
#define MOUSE_BUTTON_5 0b000100
#define MOUSE_BUTTON_PRIMARY MOUSE_BUTTON_1
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_2
#define MOUSE_BUTTON_SECONDARY MOUSE_BUTTON_3
#define MOUSE_SCROLL_UP MOUSE_BUTTON_4
#define MOUSE_SCROLL_DOWN MOUSE_BUTTON_5

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
        
    private:
        static bool initialised;
        static std::bitset<KEYBOARD_MAX*2> key_states;
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
        #ifdef _WIN32
            static HANDLE h_console;
            static HWND win_console;
            static HWND GetHwnd(void);
        #else
            static struct termios old_termios;
        #ifdef __linux__
            static struct termios old_fdterm;
            static int old_kbdmode;
            static int fd;
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
        
        static void HandleKeyboard(void);
        static bool IsKeyDown(int key);
        static bool IsKeyToggled(int key);
        static int KeyPressed(void);
        static int KeyReleased(void);

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