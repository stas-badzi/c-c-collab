#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <utility>
#include <clang_constexpr.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cwchar>
    #include <stdio.h>
#include "System.hpp"

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
    #include <tlhelp32.h>
    #include <psapi.h>
    #include <dbghelp.h>
    #include <process.h>
    #include <windows/key.hpp>
    #include <iostream>
    #include <conio.h>
    #include <windows/thread_safe/queue>
    #include <windows/thread_safe/vector>
#ifndef _MSVC
    #include <windows/quick_exit.h>
#endif
    typedef wchar_t char_t;
#else
    #include <signal.h>
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
    #include <linux/mousefd.h>
    #include <linux/input.h>
    #include <linux/fb.h>
    #include <linux/kd.h>
    #include <linux/keyboard.h>
    #include <linux/key.hpp>
    #include <linux/ledctrl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
#elif __APPLE__
    #include <apple/key.hpp>
    #include <apple/keyboard.h>
    #include <apple/openfile.h>
    #include <crt_externs.h>
    #include <libproc.h>
#elif __CYGWIN__
    #include <windows.h>
    #include <windows/key.hpp>
#else
#endif
    typedef char char_t;
#endif
#ifdef __APPLE__
    #define KEYBOARD_MAX 0x80
#else
    #define KEYBOARD_MAX 256
#endif

// change to 1 for less cpu usage but less responsiveness (windows keyboard input)
#define SLEEP_THREAD_INPUT false
// todo: make it into an cmdline trigger

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

void SysSleep(int microseconds);

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

        struct ClientInfo {
            enum class OS {
                WINDOWS,
                MSYS2,
                CYGWIN,
                LINUX,
                MACOS,
            #ifdef BSD
            #undef BSD
            #endif
                BSD
            } operating_system;

            enum class Arch {
                X64,
                ARM64,
                ARM,
                X86
            } architecture;

            enum class Term {
                conhost,
                mintty,
                //windows_terminal, // terminal emulator
                terminal_emulator,
                linux_console,
                windows_server,
                macos_recovery_treminal
            } terminal;
        } client_info;
        
    private:
        static bool initialised;
        static std::bitset<KEYBOARD_MAX> key_states;
        static int key_hit;
        static int key_released;
        static unsigned short double_click_max; // = 500;
        static std::bitset<16> mouse_buttons_down;
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
        static int pid;
        static bool sub_proc;
        static int ret;
        static std::array<bool,UINT16_MAX> used_pids;
        static uint16_t next_pid;
        static char_t buf[127]; static int8_t buf_it;
        static std::pair<int16_t,int16_t> cursorpos;
        static bool cursor_visible;
        static uint8_t cursor_size;
        static bool cursor_blink_opposite;
    #ifdef _WIN32
        static const wchar_t* subdir;
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
        static CONSOLE_CURSOR_INFO old_curinf;
        static tsqueue<wchar_t>* input_buf;
        static HANDLE input_thread;
        static void* input_thread_arg;
        static HANDLE super_thread;
        static bool* super_thread_run;
        static void* super_thread_arg;
        static bool* is_setting_cursor;
        static tsvector<HANDLE>* thread_handles;
        static inline constexpr uint8_t GenerateAtrVal(uint8_t i1, uint8_t i2);
        static DWORD WINAPI MoveCursorThread(LPVOID lpParam);
        static DWORD WINAPI SuperThread(LPVOID lpParam);
        //static std::pair<uint16_t,uint16_t> xyoffset;
        //static inline std::pair<uint16_t,uint16_t> GetXYCharOffset();
    #else
        static const char* subdir;
        static struct termios old_termios;
        static struct winsize window_size;
        static mbstate_t streammbs;
    #ifdef __linux__
        static struct termios old_fdterm;
        static int old_kbdmode;
        static int fd;
        static int fb_fd;
        static std::pair<uint32_t,uint32_t> pixelpos; 
        static input_event events[255]; static uint8_t evnts_siz;
        static int mouse_fd;
        static bool discard_mouse;
        static bool no_gpm;
        static bool parent;
        static uint8_t root_type;
        static Key::Enum key_chart[MAX_NR_KEYMAPS][KEYBOARD_MAX];
    #endif
    #ifdef __APPLE__
        static pid_t ppid;
    #endif
    #endif
        static uniconv::utfstr GetTerminalExecutableName();
        static inline char_t GetChar(void);
        static void PushChar(char_t c);
        static void XtermMouseAndFocus(void);
        static void XtermInitTracking(void);
        static void XtermFinishTracking(void);
        static void EscSeqSetTitle(const char_t* title);
        static void EscSeqMoveCursor(void);
        static void EscSeqSetCursor(void);
        static void EscSeqSetCursorSize(void);
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

        static void Update(void);

        static void Sleep(double seconds = 1.0);

        static void SetDoubleClickMaxWait(unsigned short milliseconds);
        static unsigned short GetDoubleClickMaxWait(void);

        static int PopupWindow(int type, int argc, const char_t* argv[]);
        static void MoveCursor(int x, int y);
        static void ShowCursor(void);
        static void HideCursor(void);
        static void SetCursorSize(uint8_t size);
        static void SetTitle(const char_t* title);
#ifdef _WIN32
        static std::wistringstream in;
        static std::wofstream out;
#else
        static std::wistringstream in;
        static std::ofstream out;
#endif
    };
#ifdef _WIN32
    extern __declspec(dllexport) std::wistream& gin;
    extern __declspec(dllexport) std::wostream& gout;
#else
    extern __attribute__((visibility("default"))) std::wistream& gin;
    extern __attribute__((visibility("default"))) std::ostream& gout;
#endif
} // namespace cpp