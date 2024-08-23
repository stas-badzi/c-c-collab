#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <cstdint>
#include <thread>
#include <signal.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

#include <unicode_conversion.hpp>

#ifdef _WIN32
    #include <windows.h>
    #define at_quick_exit(func) atexit(func)
    #define quick_exit(code) exit(code)
#else
    #include <stdio.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <deque>
    #include <string>
    #include <linux/kd.h>
    #include <linux/keyboard.h>
    #include <getfd.h>
#endif

#define KEYBOARD_MAX 256

namespace cpp {
    class Console {
    private:
        static bool initialised;
        static std::bitset<KEYBOARD_MAX*2> key_states;
        static int key_hit;
        static int key_released;
        #ifdef _WIN32
            static HANDLE h_console;
            static HWND win_console;
            static HWND GetHwnd(void);
        #else
            static struct termios old_termios;
            static struct termios old_fdterm;
            static int old_kbdmode;
            static int fd;
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
        static int HandleKeyboard(void);
        static bool KeyDown(int key);
        static bool KeyToggled(int key);
        static bool KeyHit(int key);
        static bool KeyReleased(int key);
        static void Sleep(double seconds = 1);
    };
} // namespace cpp