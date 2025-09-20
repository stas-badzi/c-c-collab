#pragma once

#include <tuple> // vector arm64 Ubuntu 24.04 clang compile error
#include <vector>
#include <array>
#include <bitset>
#include <utility>
#include <clang_constexpr.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <cwchar>
#include <optional>
#include <inttypes.h>
#include <stdio.h>
#include <promise.hpp>
#include <thread>
#include <atomic>

#ifdef __APPLE__
#ifndef _GLIBCXX_HAVE_AT_QUICK_EXIT // tf is this for ??????
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
    #include <io.h>
    #include <fcntl.h>
    #include <dbghelp.h>
    #include <process.h>
    #include <windows/key.hpp>
    #include <iostream>
    #include <unordered_map>
    #include <bit>
    #include <conio.h>
    #include <windows/thread_safe/queue>
    #include <windows/thread_safe/vector>
#ifndef _MSC_VER
    #include <quick_exit.h>
#else
    typedef DWORD pid_t;
#endif
    typedef wchar_t char_t;
    typedef wchar_t nchar_t;
    typedef HANDLE thread_t;
    typedef DWORD thread_ret_t;
    #define THREAD DWORD WINAPI
#else
    #include <errno.h>
    #include <signal.h>
    #include <limits>
    #include <climits>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <deque>
    #include <string>
    #include <string.h>
    #include <iostream>
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
    #define thread_t _thread_t
    #ifndef__cpp_lib_quick_exit
        #include <quick_exit.h>
    #endif
#elif __CYGWIN__
    #include <sys/cygwin.h>
    #include <sys/poll.h>
    #include <pthread.h>

    #include <windows.h>
    #include <psapi.h>
    #include <dbghelp.h>
    #include <tlhelp32.h>
    
    #include <windows/key.hpp>
    #include <unix/thread_safe/queue>
    #include <unix/thread_safe/vector>
#else
#endif
    typedef char nchar_t;
    typedef pthread_t thread_t;
    typedef void* thread_ret_t;
    #define THREAD void*
#ifdef __CYGWIN__
    typedef wchar_t char_t;
#else
    typedef char char_t;
#endif
#endif
#ifdef __APPLE__
    #define KEYBOARD_MAX 0x80
#else
    #define KEYBOARD_MAX 256
#endif

#include "System.hpp" // don't move up 'cause it breaks __CYGWIN__

#ifndef __linux__
    #define NO_CUSTOM_HANDLING /* not supported outside linux */
#endif

#ifdef NO_CUSTOM_HANDLING
    #define KEYSTATE_MAX KEYBOARD_MAX
#else
    #define KEYSTATE_MAX static_cast<uint16_t>(Key::Enum::LAST_KEY)
#endif

#ifdef _WIN32
#define geterror() GetLastError()
#else
#define geterror() errno
#endif

// change to 1 for less cpu usage but less responsiveness (windows keyboard input)
#define SLEEP_THREAD_INPUT false
// change to 1 for less cpu usage but less responsiveness (windows popup window switching)
#define SLEEP_POPUP_CHECK false
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
            std::pair<bool,bool> scroll; // (is scrolling),(up or down)[windows/linux/freebsd - scroll up == move scroll whell (fingers on touchbad) up; down == move down | macos scroll like tablet/phone (opposite)]
            unsigned int x; // in console chracters
            unsigned int y; // in console chracters
            MouseStatus(void);
        };

        struct PopupResult {
            bool finished;
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
        // max time to wait for the popup window to start up before returning -1 (in milliseconds)
        static int max_popup_startup_wait;
        static bool initialised;
        static std::mutex screen_lock;
        static std::bitset<KEYSTATE_MAX> key_states;
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
        static const nchar_t** argv;
        static struct ToggledKeys keys_toggled;
        static bool emulator;
        static pid_t pid;
        static bool sub_proc;
        static int ret;
        static std::array<bool,UINT16_MAX> used_pids;
        static uint16_t next_pid;
        static nchar_t buf[127]; static int8_t buf_it;
        static std::pair<int16_t,int16_t> cursorpos;
        static bool cursor_visible;
        static uint8_t cursor_size;
        static uniconv::tstring window_title;
        static bool cursor_blink_opposite;
        static uniconv::nstring user_data;
        static uniconv::nstring dev_data;
        static uniconv::nstring log_data;
        static uniconv::nstring tmp_data;
        static std::vector<pid_t> popup_pids;
        static rw_pipe_t parent_pipe;
        static uniconv::nstring terminal_name;
    #if defined(_WIN32) || defined(__CYGWIN__)
        static uint8_t default_fcol;
        static uint8_t default_bcol;
        static HANDLE screen;
        static HANDLE fd;
        static HWND window;
        static HWND parent_window;
        static HDC device;
        static HICON old_small_icon;
        static HICON old_big_icon;
        static HICON new_icon;
        static DWORD old_console;
        static HANDLE old_buffer;
        static CONSOLE_CURSOR_INFO old_curinf;
        static tsqueue<nchar_t>* input_buf;
        static std::atomic<bool> tabactive;
        static std::atomic<bool>* super_thread_run;
        static tsvector<thread_t>* thread_handles;
        static inline constexpr uint8_t GenerateAtrVal(uint8_t i1, uint8_t i2);
        static inline wchar_t getnch(void);
        static THREAD MoveCursorThread(void* lpParam);
        static THREAD SuperThread(void* lpParam);
    #endif
    #ifdef _WIN32
        static constexpr const wchar_t* pipedir = L"\\\\.\\pipe\\.factoryrush\\";
        static const wchar_t* subdir;
        //static std::vector<std::vector<COLORREF>> SaveScreen(void);
        //static std::pair<std::pair<uint16_t,uint16_t>,std::pair<uint16_t,uint16_t>> GetOffsetSymSize(int color1 = 3, int color2 = 9, int color3 = 1);
        
        //static std::pair<uint16_t,uint16_t> scr_offs;
        //static std::pair<uint16_t,uint16_t> sym_size;
        //static bool auto_size_updates;
        //static int16_t old_width;
        //static int16_t old_height;
        //static RECT old_rect;
        static HANDLE super_thread;
        static std::wofstream real_out;
        static size_t write_out(std::wstring str);
        //static std::pair<uint16_t,uint16_t> xyoffset;
        //static inline std::pair<uint16_t,uint16_t> GetXYCharOffset();
    #else
        static bool custom_handling;
        static std::ofstream real_out;
        static const char* subdir;
        static struct termios old_termios;
        static struct winsize window_size;
        static mbstate_t streammbs;
    #ifdef __CYGWIN__
        static pthread_t super_thread;
        static inline HWND GetHwnd(void);
    #endif
    #ifdef __linux__
        static uid_t ruid;
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
        static std::string terminal_switch;
        static Key::Enum key_chart[MAX_NR_KEYMAPS][KEYBOARD_MAX];
        static std::string GetTerminalExecuteSwitch(void);
        static void Custom_HandleKeyboard(void);
        static void Custom_GetWindowSize(void);
    #endif
    #ifdef __APPLE__
        static pid_t ppid;
    #endif
    #endif
        static uniconv::nstring GetTerminalExecutableName();
        static inline char_t GetChar(void);
        static void PushChar(char_t c);
        static void XtermMouseAndFocus(void);
        static void XtermInitTracking(void);
        static void XtermFinishTracking(void);
        static void EscSeqSetTitle(const nchar_t* title);
        static void EscSeqMoveCursor(void);
        static void EscSeqSetCursor(void);
        static void EscSeqSetCursorSize(void);
        static void EscSeqRestoreCursor(void);
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

            #if defined(_WIN32) || defined(__CYGWIN__)
                uint8_t GetAttribute(void) const;
                void SetAttribute(uint8_t attribute);
            #endif

        };
        enum class conf : bool {
            Native = true,
            Terminal = false
        };
        static struct config_t {
            conf focus;
            conf mouse;
            conf title;
            conf cursor;
            conf cursor_size;
            conf cursor_visibility;
        } config;

        static struct config_t& GetConfigRef(void);

        static int16_t GetWindowWidth(void);
        static int16_t GetWindowHeight(void);

        static int32_t GetArgC(void);
        static uniconv::utfcstr* GetArgV(void);

        static void FillScreen(const std::vector<std::vector<Symbol> >& symbols);
    #ifdef __CYGWIN__
        static void EscSeqFillScreen(const std::vector<std::vector<Symbol> >& symbols);
    #endif
        static void ClearScreenBuffer(void);
        
        static void HandleKeyboard(void);
        // Call this when you don't want to handle keyboard input to remove "key pressed" & "key released" states
        static void DontHandleKeyboard(void);
        // Call this to reset every key to not-pressed state
        static void ResetKeyboard(void);
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
        static bool IsMouseButtonDown(uint8_t button);

        static std::optional<uniconv::nstring> GetParentMessage(void);
        static bool SendParentMessage(uniconv::nstring message);
        static std::optional<uniconv::nstring> GetChildMessage(rw_pipe_t pipe);
        static bool SendChildMessage(rw_pipe_t pipe, uniconv::nstring message);

        static void HandleOutput(void);

        static void Update(void);

        static void Beep(void);

        static void Sleep(double seconds = 1.0, bool sleep_input_thread = false);
        static void Exit(int code);
        static void QuickExit(int code);
        static void SetResult(uniconv::utfcstr result);

        static void ThrowMsg(const char* msg);
        static void ThrowMsg(const wchar_t* msg);
        static void ThrowMsg(const std::string msg);
        static void ThrowMsg(const std::wstring msg);

        static void SetDoubleClickMaxWait(unsigned short milliseconds);
        static unsigned short GetDoubleClickMaxWait(void);

        static std::optional<std::pair<int,uniconv::nstring>> PopupWindow(int type, int argc, const nchar_t* argv[], const nchar_t* title = nullptr);
        static std::optional<std::pair<stsb::promise<std::optional<std::pair<int,uniconv::nstring>>>,rw_pipe_t>> PopupWindowAsync(int type, int argc, const nchar_t* argv[], const nchar_t* title = nullptr);
        static std::optional<std::pair<stsb::promise<std::optional<std::pair<int,std::u16string>>>,rw_pipe_t>> PopupWindowAsync(int type, int argc, const char16_t* argv[], const char16_t* u16title = nullptr);

        static void MoveCursor(int x, int y);
        static void ShowCursor(void);
        static void HideCursor(void);
        static void SetCursorSize(uint8_t size);
        static void SetTitle(const char_t* title);
        static void ReverseCursorBlink(void);
        static std::basic_istringstream<wchar_t> in;
        static void out_flush(void) {HandleOutput();}
        static void out_endl(void) {Console::out.put(L'\n'); HandleOutput();}
        static std::basic_ostringstream<wchar_t> out;
    private:
        static void FillScreenForce(const std::vector<std::vector<Symbol> >& symbols);
        static std::atomic<bool> refresh_screen;
        static std::vector<std::vector<Symbol>> old_symbols;
        static std::pair<int16_t,int16_t> old_scr_size;
    };
#ifdef _WIN32
    extern __declspec(dllexport) std::basic_istream<wchar_t>& win;
    extern __declspec(dllexport) std::basic_ostream<wchar_t>& wout;
#else
    extern __attribute__((visibility("default"))) std::basic_istream<wchar_t>& win;
    extern __attribute__((visibility("default"))) std::basic_ostream<wchar_t>& wout;
#endif
} // namespace cpp