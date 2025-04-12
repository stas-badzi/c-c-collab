#include "Console.hpp"
#include "dllimport.hpp"

#ifdef _MSC_VER
    #define PATH_MAX MAX_PATH
#endif

extern int Main(void);

namespace cpp {
#if defined(_WIN32)
    __declspec(dllexport) std::basic_istream<wchar_t>& win = *((std::basic_istream<wchar_t>*)&Console::in);
    __declspec(dllexport) std::basic_ostream<wchar_t>& wout = *((std::basic_ostream<wchar_t>*)&Console::out);
#define getnch() Console::input_buf->pop()
#define ncerr wcerr
#else
#define getnch() getc(stdin)
#define ncerr cerr
    __attribute__((visibility("default"))) std::basic_istream<wchar_t>& win = *((std::basic_istream<wchar_t>*)&Console::in);
    __attribute__((visibility("default"))) std::basic_ostream<wchar_t>& wout = *((std::basic_ostream<wchar_t>*)&Console::out);
#endif
}

using namespace cpp;
using namespace uniconv;
using namespace std;
using namespace std::chrono;

Console::config_t& Console::GetConfigRef(void) { return Console::config; }

void Console::ThrowMsg(const char* msg) {
    Console::Fin();
    fwrite(msg, sizeof(char), strlen(msg), stderr);
    exit(1);
}

void Console::ThrowMsg(const wchar_t* msg) {
    Console::Fin();
    fwrite(msg, sizeof(wchar_t), wcslen(msg), stderr);
    exit(0x100);
}

void Console::ThrowMsg(const string msg) {
    Console::Fin();
    fwrite(msg.c_str(), sizeof(char), msg.size(), stderr);
    exit(0x100);
}

void Console::ThrowMsg(const wstring msg) {
    Console::Fin();
    fwrite(msg.c_str(), sizeof(wchar_t), msg.size(), stderr);
    exit(0x100);
}

char_t Console::GetChar(void) {
    if (Console::buf_it >= 0 && Console::buf[Console::buf_it]) { ++buf_it; return Console::buf[buf_it-1]; }
    Console::buf[buf_it] = getnch(); if (buf_it < 127) Console::buf[++buf_it] = '\0'; else ThrowMsg("No more space in buffer");
    return Console::buf[buf_it-1];
};

void Console::PushChar(char_t c) {
    Console::in.clear();
    auto pos = Console::in.tellg();
#ifdef _WIN32
    Console::in.str(Console::in.str()+c);
#else
    //write(fileno(stdout), &c, 1);
    wchar_t wc = 0;
    size_t siz = mbrtowc(&wc, &c, 1, &Console::streammbs);
    switch (siz) {
    case static_cast<size_t>(-1):
        ThrowMsg("mbrtowc error");
    case static_cast<size_t>(-2):
        return;
    }
    Console::in.str(Console::in.str()+wc);
    char empty = 0;
    siz =  mbrtowc(&wc, &empty, 1, &Console::streammbs);
    if (siz == static_cast<size_t>(-3))
        Console::in.str(Console::in.str()+wc);
#endif
    Console::in.seekg(pos);
};

void Console::EscSeqSetTitle(const char_t* title) {
    if (title) Console::window_title = title;
    utfstr str;
    str.append(N("\033]30;")).append(window_title).append(N("\a\033]0;")).append(window_title).push_back(N('\a'));
    stderr_lock.lock();
    fwrite(str.c_str(), sizeof(char_t), str.size(), stderr);
    stderr_lock.unlock();
}

void Console::EscSeqMoveCursor(void) {
    utfstr str;
    str.append(N("\033[")).append(to_nstring(Console::cursorpos.second+1)).append(N(";")).append(to_nstring(Console::cursorpos.first+1)).push_back(N('H'));
    stderr_lock.lock();
    fwrite(str.c_str(), sizeof(char_t), str.size(), stderr);
    stderr_lock.unlock();
}

void Console::EscSeqSetCursor(void) {
    utfstr str = N("\033[?25");
    if (Console::cursor_visible) str.push_back(N('h'));
    else str.push_back(N('l'));
    stderr_lock.lock();
    fwrite(str.c_str(), sizeof(char_t), str.size(), stderr);
    stderr_lock.unlock();
}

void cpp::Console::ReverseCursorBlink(void) {
    Console::cursor_blink_opposite = !Console::cursor_blink_opposite;
    Console::EscSeqSetCursorSize();
}

void Console::EscSeqSetCursorSize(void) {
    short val;
    if (Console::cursor_size > 60) val = 2;
    else if (Console::cursor_size > 30) val = 6;
    else val = 4;
    val -= Console::cursor_blink_opposite;
    utfstr str = N("\033[");
    str.append(to_nstring(val));
    str.push_back(N(' '));
    str.push_back(N('q'));
    stderr_lock.lock();
    fwrite(str.c_str(), sizeof(char_t), str.size(), stderr);
    stderr_lock.unlock();
}

void Console::EscSeqRestoreCursor(void) {
    fwrite(N("\033[?25h\033[0 q"), sizeof(char_t), 11, stderr);
}

void Console::XtermInitTracking(void) {
    fwrite("\033[?1049h", sizeof(char), 8, stderr);

    fwrite("\033[?1004h", sizeof(char), 8, stderr);
    fwrite("\033[?1003h", sizeof(char), 8, stderr);
    fwrite("\033[?1015h", sizeof(char), 8, stderr);
    fwrite("\033[?1006h", sizeof(char), 8, stderr);
}

void Console::XtermFinishTracking(void) {
    fwrite("\033[?1004l", sizeof(char), 8, stderr);
    fwrite("\033[?1003l", sizeof(char), 8, stderr);
    fwrite("\033[?1015l", sizeof(char), 8, stderr);
    fwrite("\033[?1006l", sizeof(char), 8, stderr);
    Console::Sleep(0.01);
    Console::XtermMouseAndFocus(); // read all input so it doesn't get pushed to the terminal

    fwrite("\033[?1049l", sizeof(char), 8, stderr);
}

void Console::XtermMouseAndFocus(void) {
    Console::this_mouse_button = -1;
    Console::this_mouse_down = false;
    Console::this_mouse_combo = 0;
    Console::mouse_buttons_down[MOUSE_SCROLL_UP] = false;
    Console::mouse_buttons_down[MOUSE_SCROLL_DOWN] = false;
    Console::mouse_status.scroll = {false,false};
    bool mousedown = false;
    int bytes;

#ifdef _WIN32
    bytes = Console::input_buf->size();
#define nstrlen wcslen
#else
#define nstrlen strlen
    ioctl(STDIN_FILENO, FIONREAD, &bytes);
#endif
    if (!bytes) return;
    bytes += nstrlen(Console::buf);
    

    while (bytes > 0) {
        buf_it = 0;
        char_t c = GetChar(); --bytes;
        if (c != N('\033')) {
            Console::PushChar(c);
            buf[0] = N('\0');
            continue;
        } else if (bytes == 0) break;
        c = GetChar(); --bytes;
        if (c != N('[')) {
            Console::PushChar('\033');
            Console::PushChar(c);
            buf[0] = N('\0');
            continue;
        } else if (bytes == 0) break;
        c = GetChar(); --bytes;
        switch (c) {
        case N('I'):
            //fprintf(stderr, "gained focus\n");
            Console::focused = true;
            buf[0] = N('\0');
            continue;
        case N('O'):
            //fprintf(stderr, "lost focus\n");
            Console::focused = false;
            buf[0] = N('\0');
            continue;
        case N('<'):
            int pos = 0;
            int val[3] = {0};
            int it = 2;
            while (++it) {
                if (bytes == 0) return;
                char byte = GetChar(); --bytes;

                if (byte == N(';')) { ++pos; continue; }
                
                if (byte == N('M')) { mousedown = true; break; }
                if (byte == N('m')) { mousedown = false; break; }

                int num = byte - N('0');
                if (num < 0 || num > 9) ThrowMsg("Invalid mouse status input");

                val[pos] *= 10;
                val[pos] += num;
            };
            //fprintf(stdout, "%c", '\n');

            Console::mouse_status.x = --val[1];
            Console::mouse_status.y = --val[2];
            //fprintf(stderr, "mouse: %d %d\n", Console::mouse_status.x, Console::mouse_status.y);
            bitset<8> event(val[0]);

            if (event[5]) { ++bytes; buf[0] = N('\0'); continue; } // move event
            
            uint8_t button = 0b0;
            
            button += 0b1 * event[0];
            button += 0b10 * event[1];
            button += 0b100 * event[6];
            button += 0b1000 * event[7];
            if (button < 3 /*Button 1-3*/) ++button; // are encoded as 0-2 and there is no button 3 encoded so we add one to make it BUTTON(X) = x

            chrono::time_point<chrono::high_resolution_clock> now = chrono::high_resolution_clock::now();
            uint8_t fullbutton = 0b000000;
            fullbutton |= 0b0000001 * button; // BBBB
            fullbutton |= 0b0010000 * event[2]; // Shift
            fullbutton |= 0b0100000 * event[3]; // Meta
            fullbutton |= 0b1000000 * event[4]; // Ctrl
            // fullbutton == 0bCMSBBBB

            if (mousedown) {
                if (Console::last_mouse_button == fullbutton && now - Console::last_click_time <= chrono::milliseconds(Console::double_click_max)) {
                    if (Console::last_mouse_combo < UINT8_MAX) ++Console::last_mouse_combo;
                } else {
                    Console::last_mouse_button = (button < 3 || button > 5) ? fullbutton : -1;
                    Console::last_mouse_combo = 1;
                }
                Console::last_click_time = now;
            }

            Console::this_mouse_combo = ( Console::last_click_time == now ) ? Console::last_mouse_combo : 1;
            Console::this_mouse_down = mousedown;
            Console::mouse_buttons_down[button] = this_mouse_down;
            Console::this_mouse_button = button;
            
            switch (button) {
            case 1:
                Console::mouse_status.primary = mousedown;
                break;
            case 2:
                Console::mouse_status.middle = mousedown;
                break;
            case 3:
                Console::mouse_status.secondary = mousedown;
                break;
            case 4:
                Console::mouse_status.scroll = {true,true};
                break;
            case 5:
                Console::mouse_status.scroll = {true,false};
                break;
            }
            buf[0] = N('\0');
            continue;
        }
        Console::PushChar('\033');
        Console::PushChar('[');
        Console::PushChar(c);
        buf[0] = N('\0');
    }
}

    static bool tabby = false;
#ifdef _WIN32
    static bool conhost = false;
    static bool mintty = false;
    static bool wt = false;
    static bool conemu = false;
    static DWORD conemupid = 0;
    static bool cmder = false;
    static bool good = false;

    inline constexpr uint8_t Console::GenerateAtrVal(uint8_t i1, uint8_t i2) {
        uint8_t val = 0x0000;
        if (i1 == 0) { val |= 0x0000; }
        if (i1 == 1) { val |= FOREGROUND_RED; }
        if (i1 == 2) { val |= FOREGROUND_GREEN; }
        if (i1 == 3) { val |= FOREGROUND_RED | FOREGROUND_GREEN; }
        if (i1 == 4) { val |= FOREGROUND_BLUE; }
        if (i1 == 5) { val |= FOREGROUND_RED | FOREGROUND_BLUE; }
        if (i1 == 6) { val |= FOREGROUND_BLUE | FOREGROUND_GREEN; }
        if (i1 == 7) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; }
        if (i1 == 8) { val |= FOREGROUND_INTENSITY; }
        if (i1 == 9) { val |= FOREGROUND_RED | FOREGROUND_INTENSITY; }
        if (i1 == 10) { val |= FOREGROUND_GREEN | FOREGROUND_INTENSITY; }
        if (i1 == 11) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; }
        if (i1 == 12) { val |= FOREGROUND_BLUE | FOREGROUND_INTENSITY; }
        if (i1 == 13) { val |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; }
        if (i1 == 14) { val |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; }
        if (i1 == 15) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; }
        if (i1 >= 16) { val |= Console::default_fcol * 0b00000001; }
        if (i2 == 0) { val |= 0x0000; }
        if (i2 == 1) { val |= BACKGROUND_RED; }
        if (i2 == 2) { val |= BACKGROUND_GREEN; }
        if (i2 == 3) { val |= BACKGROUND_RED | BACKGROUND_GREEN; }
        if (i2 == 4) { val |= BACKGROUND_BLUE; }
        if (i2 == 5) { val |= BACKGROUND_RED | BACKGROUND_BLUE; }
        if (i2 == 6) { val |= BACKGROUND_BLUE | BACKGROUND_GREEN; }
        if (i2 == 7) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; }
        if (i2 == 8) { val |= BACKGROUND_INTENSITY; }
        if (i2 == 9) { val |= BACKGROUND_RED | BACKGROUND_INTENSITY; }
        if (i2 == 10) { val |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; }
        if (i2 == 11) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY; }
        if (i2 == 12) { val |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; }
        if (i2 == 13) { val |= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY; }
        if (i2 == 14) { val |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY; }
        if (i2 == 15) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY; }
        if (i2 >= 16) { val |= Console::default_bcol * 0b00010000; }
        return val;
    }

    // first foreground second background
    inline constexpr pair<uint8_t,uint8_t> AtrValToColors(uint8_t val) {
        pair<uint8_t,uint8_t> out;

        out.first = 0x0000;
        if ( (val & 0b00001111) == (FOREGROUND_RED) ) { out.first = 1; }
        if ( (val & 0b00001111) == (FOREGROUND_GREEN) ) { out.first = 2; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_GREEN) ) { out.first = 3; }
        if ( (val & 0b00001111) == (FOREGROUND_BLUE) ) { out.first = 4; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_BLUE) ) { out.first = 5; }
        if ( (val & 0b00001111) == (FOREGROUND_BLUE | FOREGROUND_GREEN) ) { out.first = 6; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ) { out.first = 7; }
        if ( (val & 0b00001111) == (FOREGROUND_INTENSITY) ) { out.first = 8; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_INTENSITY) ) { out.first = 9; }
        if ( (val & 0b00001111) == (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.first = 10; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.first = 11; }
        if ( (val & 0b00001111) == (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.first = 12; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.first = 13; }
        if ( (val & 0b00001111) == (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.first = 14; }
        if ( (val & 0b00001111) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.first = 15; }

        out.second = 0x0000;
        if ( (val & 0b11110000) == (FOREGROUND_RED) ) { out.second = 1; }
        if ( (val & 0b11110000) == (FOREGROUND_GREEN) ) { out.second = 2; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_GREEN) ) { out.second = 3; }
        if ( (val & 0b11110000) == (FOREGROUND_BLUE) ) { out.second = 4; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_BLUE) ) { out.second = 5; }
        if ( (val & 0b11110000) == (FOREGROUND_BLUE | FOREGROUND_GREEN) ) { out.second = 6; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ) { out.second = 7; }
        if ( (val & 0b11110000) == (FOREGROUND_INTENSITY) ) { out.second = 8; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_INTENSITY) ) { out.second = 9; }
        if ( (val & 0b11110000) == (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.second = 10; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.second = 11; }
        if ( (val & 0b11110000) == (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.second = 12; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.second = 13; }
        if ( (val & 0b11110000) == (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ) { out.second = 14; }
        if ( (val & 0b11110000) == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ) { out.second = 15; }

        return out;
    }

    inline constexpr COLORREF GetPixel(int x, int y, int width, int height, RGBQUAD* pixels) {
        return RGB(pixels[(height-y-1)*width + x].rgbRed, pixels[(height-y-1)*width + x].rgbGreen, pixels[(height-y-1)*width + x].rgbBlue);
    }
    
    struct __getinput_arg { tsqueue<wchar_t>* buf; bool dowait; };

    DWORD WINAPI InputThread(LPVOID pvarg) {
        __getinput_arg* arg = (__getinput_arg*)pvarg;
        tsqueue<wchar_t>& buf = *(arg->buf);
        bool dowait = arg->dowait;
        delete arg;
        while(1) {
            while (_kbhit()) {
                wchar_t wc = _getwch();
                if (!wc) { if (_kbhit()) {
                    wc = _getwch();
                    switch (wc) {
                    }
                } else {
                    wc = _getwch();
                    switch (wc) {
                    }
                    continue;
                } }

                if (wc == 0xe0) { if (_kbhit()) {
                    wc = _getwch();
                    switch (wc) {
                    case 0x48: wcerr << L'↑'; continue;
                    case 0x50: wcerr << L'↓'; continue;
                    case 0x4b: wcerr << L'←'; continue;
                    case 0x4d: wcerr << L'→'; continue;
                    default: buf.push('\xe0'); break;
                    }
                } else {
                    wc = _getwch();
                    switch (wc) {
                        case 0x48: wcerr << L'↑'; break;
                        case 0x50: wcerr << L'↓'; break;
                        case 0x4b: wcerr << L'←'; break;
                        case 0x4d: wcerr << L'→'; break;
                    }
                    continue;
                } }
                buf.push(wc);
            }
            if (dowait) ::Sleep(1);
        }
        return TRUE;
    }

    size_t sleepmcs(size_t mcs) {
        size_t i = 0;
        auto start = std::chrono::high_resolution_clock::now();
        while (std::chrono::high_resolution_clock::now() - start < std::chrono::microseconds(mcs)) ++i;
        return i;
    }

    struct __conemuhandletabs_arg { atomic<bool>* focused; DWORD pid; };

    DWORD WINAPI ConemuHandleTabs(LPVOID lpParam) {
        auto args = (__conemuhandletabs_arg*)lpParam;
        atomic<bool>& focused = *(args->focused);
        DWORD pid = (args->pid);
        delete args;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (hProcess == NULL) {
            std::cerr << "Failed to open process with ID " << pid << ". Error: " << GetLastError() << std::endl;
            return 1;
        }
        IO_COUNTERS prevIoCounters = {};
        if (!GetProcessIoCounters(hProcess, &prevIoCounters)) {
            std::cerr << "Failed to get IO counters for process with ID " << pid << ". Error: " << GetLastError() << std::endl;
            exit(3);
        }

        ULONGLONG old_writeDelta = 0;
        ULONGLONG old_readDelta = 0;
        ULONGLONG old_otherOpDelta = 0;
        ULONGLONG old_readTransferDelta = 0;
        ULONGLONG old_writeTransferDelta = 0;
        ULONGLONG old_otherTransferDelta = 0;

        ULONGLONG last_deltas[1'000] = {100};
        ULONGLONG deltas_sum = 0;
        size_t deltas_it = 0;
        ULONGLONG old_delta = 0;

        ULONGLONG last_vals[1'000] = {0};
        size_t vals_it = 0;

        long double last_percents[1'000] = {0};
        long double avgpercent = 0;
        size_t perc_it = 0;

        int oneconter = 0;

        focused = true;

        while (true) {
            sleepmcs(900);
            IO_COUNTERS ioCounters;
            if (GetProcessIoCounters(hProcess, &ioCounters)) {
                auto calculateDelta = [](ULONGLONG newVal, ULONGLONG oldVal) {
                    return newVal - oldVal;
                };

                auto calculatePercentage = [](ULONGLONG delta, ULONGLONG oldVal) {
                    if (delta == 0 && oldVal == 0) return (long double)0;
                    return delta < oldVal ? ((static_cast<long double>(delta)-oldVal) / oldVal) * 100 : ((static_cast<long double>(oldVal)-delta) / delta) * -100;
                };

                ULONGLONG writeDelta = calculateDelta(ioCounters.WriteOperationCount, prevIoCounters.WriteOperationCount);
                ULONGLONG readDelta = calculateDelta(ioCounters.ReadOperationCount, prevIoCounters.ReadOperationCount);
                ULONGLONG otherOpDelta = calculateDelta(ioCounters.OtherOperationCount, prevIoCounters.OtherOperationCount);
                ULONGLONG readTransferDelta = calculateDelta(ioCounters.ReadTransferCount, prevIoCounters.ReadTransferCount);
                ULONGLONG writeTransferDelta = calculateDelta(ioCounters.WriteTransferCount, prevIoCounters.WriteTransferCount);
                ULONGLONG this_delta = calculateDelta(ioCounters.OtherTransferCount, prevIoCounters.OtherTransferCount);

                auto delta = deltas_sum;
                auto percentup = calculatePercentage(delta, last_vals[vals_it]);

                old_writeDelta = writeDelta;
                old_readDelta = readDelta;
                old_otherOpDelta = otherOpDelta;
                old_readTransferDelta = readTransferDelta;
                old_writeTransferDelta = writeTransferDelta;
                old_otherTransferDelta = this_delta;

                deltas_sum -= last_deltas[deltas_it];
                deltas_sum += this_delta;
                last_deltas[deltas_it] = this_delta;

                avgpercent -= last_percents[perc_it];
                last_percents[perc_it] = percentup;
                avgpercent += percentup;
                last_vals[vals_it] = delta;

                prevIoCounters = ioCounters;
                deltas_it = (deltas_it + 1) % 1'000;
                perc_it = (perc_it + 1) % 1'00;
                vals_it = (vals_it + 1) % 1'000;


                if (avgpercent > 70e2) focused = true;
                else if (avgpercent < -70e2) focused = false;

            } else {
                cerr << "GetProcessIoCounters failed: " << GetLastError() << endl;
                exit(1);
            }
        }

        CloseHandle(hProcess);
    }

    struct __superthread_arg { atomic<bool>* dorun; tsvector<HANDLE>* threads; };

    DWORD WINAPI Console::SuperThread(LPVOID lpParam) {
        auto args = (__superthread_arg*)lpParam;
        atomic<bool>& run = *(args->dorun);
        tsvector<HANDLE>& threads = *(args->threads);
        delete args;
        while (run) {
            for (size_t i = 0; i < threads.size(); i++) {
                auto res = WaitForSingleObject(threads[i], 0);
                if (res == WAIT_OBJECT_0) {
                    CloseHandle(threads[i]);
                    threads.erase(i);
                }
                else if (res != WAIT_TIMEOUT) {
                    Console::out << L"WaitForSingleObject failed: " << GetLastError(); out_endl();
                    return FALSE;
                }
            }
            ::Sleep(1);
        }
        for (size_t i = 0; i < threads.size(); ++i) {
            TerminateThread(threads[i], 0);
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]);
        }
        return TRUE;
    }

    // slow function so we put it in a separate thread
    // trns out is wasn't slow but it can stay in here for now
    DWORD WINAPI Console::MoveCursorThread(LPVOID lpParam) {
        bool& issetting = *(bool*)lpParam;
        while (issetting) ::Sleep(1);
        issetting = true;
        if (!SetConsoleCursorPosition(Console::screen, {cursorpos.first, cursorpos.second})) {
            issetting = false;
            Console::out << L"SetCursorPos failed: " << GetLastError(); out_endl();
            return FALSE;
        }
        issetting = false;
        return TRUE;
    }

    void cpp::Console::MoveCursor(int x, int y) {
        Console::cursorpos.first = x;
        Console::cursorpos.second = y;
        //HANDLE hThread = CreateThread(NULL, 0, MoveCursorThread, Console::is_setting_cursor, 0, NULL);
        //thread_handles->push_back(hThread);
        screen_lock.lock();
        SetConsoleCursorPosition(Console::screen, {cursorpos.first, cursorpos.second});
        screen_lock.unlock();
    }

    void cpp::Console::ShowCursor(void) {
        Console::cursor_visible = true;

        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = Console::cursor_visible;
        cci.dwSize = Console::cursor_size;
        screen_lock.lock();
        SetConsoleCursorInfo(Console::screen, &cci);
        screen_lock.unlock();
    }

    void cpp::Console::HideCursor(void) {
        Console::cursor_visible = false;

        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = Console::cursor_visible;
        cci.dwSize = Console::cursor_size;
        screen_lock.lock();
        SetConsoleCursorInfo(Console::screen, &cci);
        screen_lock.unlock();
    }

    void cpp::Console::SetCursorSize(uint8_t size) {
        Console::cursor_size = size;

        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = Console::cursor_visible;
        cci.dwSize = Console::cursor_size;
        screen_lock.lock();
        SetConsoleCursorInfo(Console::screen, &cci);
        screen_lock.unlock();
    }

    void cpp::Console::SetTitle(const wchar_t* title) {
        if (title) Console::window_title = title;
        if (!SetConsoleTitle(Console::window_title.c_str())) {
            Console::out << L"SetConsoleTitle failed: " << GetLastError(); out_endl();
            ThrowMsg("SetConsoleTitle failed");
        }
        if (cmder) {
            wstring conemuc = GetTerminalExecutableName();
            while (conemuc.back() != L'\\') conemuc.pop_back();
            conemuc += L"vendor\\conemu-maximus5\\ConEmu\\ConEmuC";
        #ifdef _WIN64
            conemuc += L"64";
        #endif
            conemuc += L".exe";

            wstring args_v = L"\""; args_v.append(conemuc).append(L"\" -GUIMacro:").append(to_wstring(conemupid)).append(L" Rename(0,\"").append(title).append(L"\")");
            PROCESS_INFORMATION pi = PROCESS_INFORMATION();
            STARTUPINFO si = STARTUPINFO();
            si.cb = sizeof(STARTUPINFO);
            int status = CreateProcess(conemuc.c_str(), (wchar_t*)args_v.c_str(), nullptr, nullptr, false, CREATE_PRESERVE_CODE_AUTHZ_LEVEL | CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
            
            if (!status) { cerr << "CreateProcess failed: " << GetLastError() << endl; exit(0x63); }
            CloseHandle(pi.hThread);
            WaitForSingleObject(pi.hProcess, INFINITE);
            bool isdone = false;
            DWORD exitcode = -1;
            if (GetExitCodeProcess(pi.hProcess, &exitcode)) {
                if (exitcode)
                    wcerr << conemuc << L" exited with code: " << exitcode << endl;
                CloseHandle(pi.hProcess);
            } else {
                CloseHandle(pi.hProcess);
                cerr << "GetExitCodeProcess failed: " << GetLastError() << endl;
                exit(0x66);
            }
        }
    }

    vector<DWORD> GetChildProcessIds(DWORD parentProcId) {
        vector<DWORD> vec; int i = 0;
        HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe = PROCESSENTRY32();
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hp, &pe)) {
            do {
            if (pe.th32ParentProcessID == parentProcId) {
                vec.push_back(pe.th32ProcessID); i++;
            }
            } while (Process32Next(hp, &pe));
        }
        CloseHandle(hp);
        return vec;
    }

    DWORD GetParentProcessId(DWORD childProcId) {
        HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe = PROCESSENTRY32();
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hp, &pe)) {
            do {
                if (pe.th32ProcessID == childProcId) {
                    return pe.th32ParentProcessID;
                }
            } while (Process32Next(hp, &pe));
        }
        CloseHandle(hp);
        return 0;
    }

    #define popback(str, n) str.erase(str.size()-n)

    std::wstring ModifyExecutableName(std::wstring term) {
        if (term.size() > 10 && term.substr(term.size()-10) == L"\\Tabby.exe") {
            wt = false;
            tabby = true; // todo - do it for linux
        }
        if (term.size() > 19 && term.substr(term.size()-19) == L"\\ConEmu\\ConEmuC.exe") {
            term = term.substr(0,term.size()-19);
            term.append(L"ConEmu64.exe");
            wt = false;
            conemu = true;
        }
        if (term.size() > 21 && term.substr(term.size()-21) == L"\\ConEmu\\ConEmuC64.exe") {
            term = term.substr(0,term.size()-20);
            term.append(L"ConEmu64.exe");
            wt = false;
            conemu = true;
        }
        if (term.size() > 34 && term.substr(term.size()-34) == L"\\vendor\\conemu-maximus5\\ConEmu.exe") {
            term = term.substr(0,term.size()-33);
            term.append(L"Cmder.exe");
            wt = false;
            cmder = true;
            conemu = true;
        }
        if (term.size() > 36 && term.substr(term.size()-36) == L"\\vendor\\conemu-maximus5\\ConEmu64.exe") {
            term = term.substr(0,term.size()-35);
            term.append(L"Cmder.exe");
            wt = false;
            cmder = true;
            conemu = true;
        }
        return term;
    }

    bool FocusWindow(HWND hwnd) {
        int style = GetWindowLong(hwnd, GWL_STYLE);
        if ((style & WS_MINIMIZE) == WS_MINIMIZE)
            ShowWindow(hwnd, SW_RESTORE);

        DWORD focused_thread = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
        DWORD this_thread = GetCurrentThreadId();

        if (focused_thread != this_thread)
        {
            AttachThreadInput(focused_thread, this_thread, true);
            BringWindowToTop(hwnd);
            auto res = ShowWindow(hwnd, SW_SHOW);
            AttachThreadInput(focused_thread, this_thread, false);
            return res;
        } else {
            BringWindowToTop(hwnd);
            return ShowWindow(hwnd, SW_SHOW);
        }
    }

    std::wstring GetProcessExecutableName(DWORD processId, bool ischild = false, bool isparent = false) {
        Console::out << L"Process: " << processId << L'\n' << L'\t';
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (processHandle) {
            wchar_t exePath[MAX_PATH];
            if (GetModuleFileNameEx(processHandle, NULL, exePath, MAX_PATH)) {
                Console::out << L"Found name: " << exePath << L'\n' << L'\t';
                CloseHandle(processHandle);
                auto path = ModifyExecutableName(exePath);
                Console::out << L"Modified name: " << path << L'\n' << L'\t' << flush;

                HMODULE moduleHandle = LoadLibrary(path.c_str());
                if (!moduleHandle) {
                    int err = GetLastError();
                    Console::out << L"Failed to load module: " << err; Console::out_endl();
                    if (err == ERROR_ACCESS_DENIED || err == ERROR_BAD_EXE_FORMAT) {
                        Console::out << L"Access denied" << L'\n' << L'\t' << flush;
                        return path;
                    }
                    return std::wstring();
                }
                //cerr << "Handle: " << moduleHandle << L'\n' << '\t';
                PIMAGE_NT_HEADERS nth = ImageNtHeader((PVOID)moduleHandle);
                if (!nth) return std::wstring();
                //cerr << "Image NT Header: " << nth << L'\n' << '\t';
                if (nth->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI || nth->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_OS2_CUI || nth->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_POSIX_CUI) {  
                    Console::out << L"Subsystem: Console" << L'\n' << L'\t' << flush;
                    // Check child processes
                    auto childProcessIds = GetChildProcessIds(processId);
                    if (!isparent)
                        for (auto&& childProcessId : childProcessIds) {
                            std::wstring childExeName = GetProcessExecutableName(childProcessId,true);
                            if (!childExeName.empty())
                                return childExeName;
                        }
                    // Check parent process
                    if (!ischild) {
                        DWORD parentProcessId = GetParentProcessId(processId);
                        std::wstring parentExeName = GetProcessExecutableName(parentProcessId,false,true);
                        if (!parentExeName.empty())
                            return parentExeName;
                    }
                    Console::out << L"None Found"; Console::out_endl();
                    return (ischild || isparent) ? std::wstring() : path;
                }
                Console::out << L"Subsystem: GUI" << L'\n' << L'\t' << flush;
                return path;
            }
            CloseHandle(processHandle);
        }
        return std::wstring();
    }

    std::wstring GetWindowExecutableName(HWND hwnd) {
        Console::out << L"Window: " << hwnd << L'\n';
        DWORD processId;
        GetWindowThreadProcessId(hwnd, &processId);
        auto term = GetProcessExecutableName(processId);
        conhost = term.size() > 12 && term.substr(term.size()-12) == L"\\conhost.exe";
        return term;
    }

    uniconv::utfstr Console::GetTerminalExecutableName(void) { return Console::terminal_name; }


/*
    vector<vector<COLORREF>> Console::SaveScreen(void) {

        auto oldwinlong = GetWindowLong(Console::window, GWL_STYLE);
        SetWindowLong(Console::window, GWL_STYLE, oldwinlong & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

        POINT begin,end;
        RECT r;
        GetClientRect(Console::window,&r);
        begin = {r.left,r.top}; ClientToScreen(Console::window,&begin);
        end = {r.right,r.bottom}; ClientToScreen(Console::window,&end);

        int const width = end.x - begin.x;
        int const height = end.y - begin.y;

        HDC memory = CreateCompatibleDC(Console::device);
        HBITMAP const bitmap = CreateCompatibleBitmap(Console::device, width, height);
        auto const old_bitmap = SelectObject(memory, bitmap);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biSize = sizeof(BITMAPINFO);

        RGBQUAD* pixels = new RGBQUAD[height * width];
        BitBlt(memory, 0, 0, width, height, Console::device, begin.x, begin.y, SRCCOPY);
        GetDIBits(memory, bitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);

        vector<vector<COLORREF>> colors;
        for (int i = 0; i < height; ++i) { colors.push_back(vector<COLORREF>()); for (int j = 0; j < width; ++j) colors.back().push_back(GetPixel(j,i,width,height,pixels)); }
        delete[] pixels;

        SetWindowLong(Console::window, GWL_STYLE, oldwinlong);
        SelectObject(memory, old_bitmap);
        DeleteObject(bitmap);
        DeleteDC(memory);
        ReleaseDC(0, device);

        return colors;
    }


    pair<pair<uint16_t,uint16_t>,pair<uint16_t,uint16_t>> Console::GetOffsetSymSize(int color1, int color2, int color3) {
        
    //~setup   

        WORD color_atr1, color_atr2, color_atr3;
        switch (color1) {
        case 0:
            color_atr1 = 0;
            break;
        case 1:
            color_atr1 = BACKGROUND_BLUE;
            break;
        case 2:
            color_atr1 = BACKGROUND_GREEN;
            break;
        case 3:
            color_atr1 = BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 4:
            color_atr1 = BACKGROUND_RED;
            break;
        case 5:
            color_atr1 = BACKGROUND_BLUE | BACKGROUND_RED;
            break;
        case 6:
            color_atr1 = BACKGROUND_GREEN | BACKGROUND_RED;
            break;
        case 7:
            color_atr1 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 8:
            color_atr1 = BACKGROUND_INTENSITY;
            break;
        case 9:
            color_atr1 = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case 10:
            color_atr1 = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 11:
            color_atr1 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 12:
            color_atr1 = BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 13:
            color_atr1 = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 14:
            color_atr1 = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 15:
            color_atr1 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        default:
            exit(7);
        }
        switch (color2) {
        case 0:
            color_atr2 = 0;
            break;
        case 1:
            color_atr2 = BACKGROUND_BLUE;
            break;
        case 2:
            color_atr2 = BACKGROUND_GREEN;
            break;
        case 3:
            color_atr2 = BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 4:
            color_atr2 = BACKGROUND_RED;
            break;
        case 5:
            color_atr2 = BACKGROUND_BLUE | BACKGROUND_RED;
            break;
        case 6:
            color_atr2 = BACKGROUND_GREEN | BACKGROUND_RED;
            break;
        case 7:
            color_atr2 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 8:
            color_atr2 = BACKGROUND_INTENSITY;
            break;
        case 9:
            color_atr2 = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case 10:
            color_atr2 = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 11:
            color_atr2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 12:
            color_atr2 = BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 13:
            color_atr2 = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 14:
            color_atr2 = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 15:
            color_atr2 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        default:
            exit(7);
        }
        switch (color3) {
        case 0:
            color_atr3 = 0;
            break;
        case 1:
            color_atr3 = BACKGROUND_BLUE;
            break;
        case 2:
            color_atr3 = BACKGROUND_GREEN;
            break;
        case 3:
            color_atr3 = BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 4:
            color_atr3 = BACKGROUND_RED;
            break;
        case 5:
            color_atr3 = BACKGROUND_BLUE | BACKGROUND_RED;
            break;
        case 6:
            color_atr3 = BACKGROUND_GREEN | BACKGROUND_RED;
            break;
        case 7:
            color_atr3 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
            break;
        case 8:
            color_atr3 = BACKGROUND_INTENSITY;
            break;
        case 9:
            color_atr3 = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case 10:
            color_atr3 = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 11:
            color_atr3 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case 12:
            color_atr3 = BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 13:
            color_atr3 = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 14:
            color_atr3 = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case 15:
            color_atr3 = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        default:
            exit(7);
        }
    
        CONSOLE_CURSOR_INFO cci, n_cci;
        GetConsoleCursorInfo(Console::screen,&cci);
        n_cci = cci;
        n_cci.bVisible = false;
        SetConsoleCursorInfo(Console::screen,&n_cci);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(Console::screen, &csbi);
        int char_width = Console::GetWindowWidth();
        int char_height = Console::GetWindowHeight();

        CONSOLE_SCREEN_BUFFER_INFOEX csbix{0};
        csbix.cbSize = sizeof(csbix);
        GetConsoleScreenBufferInfoEx(Console::screen, &csbix);
        COLORREF color_ref1 = csbix.ColorTable[color1];
        COLORREF color_ref2 = csbix.ColorTable[color2];
        COLORREF color_ref3 = csbix.ColorTable[color3];

        bool IsWindowsTerminal = getenv("WT_SESSION") != NULL;
        bool IsMintty = string(getenv("TERM_PROGRAM")) == "mintty";

    //~setup

        DWORD written;
        wchar_t* screen_chars = new wchar_t[char_width*char_height];
        WORD* screen_atrs = new WORD[char_width*char_height];
        wchar_t* new_screen_chars = new wchar_t[char_width*char_height];
        WORD* new_screen_atrs = new WORD[char_width*char_height];

        ReadConsoleOutputCharacter(Console::screen, screen_chars, char_width*char_height, {0,0}, &written);
        ReadConsoleOutputAttribute(Console::screen, screen_atrs, char_width*char_height, {0,0}, &written);
        for (int i = 0; i < char_height; ++i) for (int j = 0; j < char_width; ++j) new_screen_chars[i*char_width + j] = (i*j == 0) ? L' ' : screen_chars[i*char_width+j];
        for (int i = 0; i < char_height; ++i) for (int j = 0; j < char_width; ++j) new_screen_atrs[i*char_width + j] = (i == 0) ? (j == 0) ? color_atr3 : color_atr1 : (j == 0) ? color_atr2 : screen_atrs[i*char_width+j];
        
        WriteConsoleOutputCharacter(Console::screen, new_screen_chars, char_width*char_height, {0, 0}, &written);
        WriteConsoleOutputAttribute(Console::screen, new_screen_atrs, char_width*char_height, {0, 0}, &written);
        auto img = Console::SaveScreen();
        WriteConsoleOutputCharacter(Console::screen, screen_chars, char_width*char_height, {0, 0}, &written);
        WriteConsoleOutputAttribute(Console::screen, screen_atrs, char_width*char_height, {0, 0}, &written);

        delete[] screen_chars;
        delete[] screen_atrs;
        delete[] new_screen_chars;
        delete[] new_screen_atrs;

        int const width = img[0].size();
        int const height = img.size();
        int const minxy = min(width,height);

        pair<uint16_t,uint16_t> xyoffset = pair<uint16_t,uint16_t>(-1,-1);
        bool IsColumn;
        for (uint16_t l = 0; l < minxy*2; l++) {
            for (uint16_t j = 0; j < minxy - l; j++) {
                int x = j,y = j;
                if ( (l+1) % 2) x += l/2;
                else y += l/2;
                COLORREF col = img[y][x];
                if (color_ref1 == col || color_ref2 == col || color_ref3 == col) {
                    xyoffset.first = x;
                    xyoffset.second = y;
                    IsColumn = !(color_ref1 == col);
                    goto GetOffsetBreakLoop;
                }
            }
        }
        return Console::GetOffsetSymSize(color1,color2,color3);
    GetOffsetBreakLoop:
        if (IsColumn) goto GetOffsetFindColumn;
    GetOffsetFindRow:
    // 0 stays 0
        for (uint16_t i = xyoffset.first; i > 0; i--) {
            COLORREF col = img[xyoffset.second][i];
            if (col != color_ref1 && col != color_ref3) {
                xyoffset.first = i;
                break;
            }
        }

        if (IsColumn) goto GetOffsetFindFinish;
    GetOffsetFindColumn:
    // 0 stays 0

        for (uint16_t i = xyoffset.second; i > 0; i--) {
            COLORREF col = img[i][xyoffset.first + 1];
            if (col != color_ref2 && col != color_ref3) {
                xyoffset.second = i;
                break;
            }
        }

        if (IsColumn) goto GetOffsetFindRow;
    GetOffsetFindFinish:

    // GetSymSize
        pair<uint16_t,uint16_t> symsize = pair<uint16_t,uint16_t>(-1,-1);

        for (uint16_t i = xyoffset.first + 1; i < img.size(); i++) {
            COLORREF col = img[xyoffset.second + 1][i];
            if (col != color_ref3) {
                if (IsWindowsTerminal) { //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    QUERY_USER_NOTIFICATION_STATE pquns; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    SHQueryUserNotificationState(&pquns); //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    if (pquns != QUNS_BUSY && xyoffset.first > 5) --i; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                } //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                else if (xyoffset.first > 5) --i; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                symsize.first = i - xyoffset.first;
                break;
            }
        }

        for (uint16_t i = xyoffset.second + 1; i < img.size(); i++) {
            COLORREF col = img[i][xyoffset.first + 1];
            if (col != color_ref3) {
                if (IsWindowsTerminal) { //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    QUERY_USER_NOTIFICATION_STATE pquns; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    SHQueryUserNotificationState(&pquns); //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                    if (pquns == QUNS_BUSY || xyoffset.second > 5) --i; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                } //for now fix later!!!!!!!!!!!!!!!!!
                else if (xyoffset.second > 5) --i; //for now fix later!!!!!!!!!!!!!!!!! (hard-coded values are bad!!!!!!!!!!)
                symsize.second = i - xyoffset.second;
                break;
            }
        }

        if (symsize.first == symsize.second) ++symsize.second; // only for now i should find a fix for this (hard-coded ifs are bad!!!!!!!!!!)

        SetConsoleCursorInfo(screen,&cci);
        
        if(!symsize.first || !symsize.second) return GetOffsetSymSize(color1,color2,color3);
        return pair<pair<uint16_t,uint16_t>,pair<uint16_t,uint16_t>>(xyoffset,symsize);
    }
*/
    struct NameHwnd {
        wchar_t* name;
        HWND hwnd;
    };

    BOOL CALLBACK FindWindowBegin(_In_ HWND   hwnd, _In_ LPARAM lParam) {
        NameHwnd* namehwnd = (NameHwnd*)lParam;
        wchar_t* pszFindWindow = namehwnd->name;
        wchar_t pszWindowTitle[1024];
        GetWindowText(hwnd, pszWindowTitle, 1024);
        bool right = true;
        if (wcslen(pszWindowTitle) < wcslen(pszFindWindow)) return TRUE;
        for (size_t i = 0; i < wcslen(pszFindWindow); ++i) {
            if (pszWindowTitle[i] != pszFindWindow[i]) {
                right = false;
                break;
            }
        }
        //wcout << pszWindowTitle << L' ' << pszFindWindow << ':' << right << L'\n';
        if (!right) return TRUE;
        if (wcslen(pszWindowTitle) == wcslen(pszFindWindow) || !isdigit(pszWindowTitle[wcslen(pszWindowTitle)])) {
            namehwnd->hwnd = hwnd;
            return FALSE;
        }
        return TRUE;
    }

    inline HWND GetHwnd(void) {
        HWND hwndFound;
        wchar_t pszNewWindowTitle[1024];
        wsprintf(pszNewWindowTitle,L"%d/%d",GetTickCount(),GetCurrentProcessId());
        SetConsoleTitle(pszNewWindowTitle);
        SysSleep(40e3);
        hwndFound=FindWindow(NULL, pszNewWindowTitle);
        if (!(good = hwndFound)) {
            if (mintty) {
                NameHwnd namehwnd = {pszNewWindowTitle, 0};
                BOOL ret = EnumWindows(FindWindowBegin, (LPARAM)&namehwnd);
                Console::out << L"EnumWindows: " << ret << L'\n';
                if (!ret) {
                    good = true;
                    hwndFound = namehwnd.hwnd;
                    Console::out << L"Window: " << hwndFound << L" good: " << good << L'\n';
                    return hwndFound;
                }
            }
            hwndFound = GetForegroundWindow();
            wchar_t pszWindowTitle[1024];
            int siz = GetWindowText(hwndFound, pszWindowTitle, 1024);
            pszWindowTitle[siz] = L'\0';
            if (wcscmp(pszWindowTitle, pszNewWindowTitle) == 0) good = true;
        }
        Console::out << L"Window: " << hwndFound << L" good: " << good << L'\n';
        return hwndFound;
    }
    void Console::Init(void) {
        if (!initialised) {
            auto term_prog = _wgetenv(L"TERM_PROGRAM");
            mintty = term_prog && wstring(term_prog) == L"mintty";
            wt = _wgetenv(L"WT_SESSION") != NULL;
            cmder = _wgetenv(L"CMDER_ROOT") != NULL;
            conemu = _wgetenv(L"ConEmuBuild") != NULL;


            bool fixmintty = mintty;
            int sub_process = 0;
            auto cmdline = GetCommandLine();
            auto winargv = (const wchar_t**)CommandLineToArgvW(cmdline, &Console::argc);
            Console::argv = (const wchar_t**)malloc(sizeof(wchar_t*) * Console::argc);
            int j = 0;
            for (int i = 0; i < Console::argc; i++) {
                auto&& arg = winargv[i+j];
                if (wcslen(arg) > 1 && arg[0] == L'\033') {
                    wstring sdir; const wchar_t* narg;
                    switch (arg[1]) {
                        case L'&':
                            // launched as popup
                            fixmintty = false;
                            Console::sub_proc = true;
                            if (wcslen(arg) < 3) ThrowMsg("Invalid argument 1");
                            sub_process = 0;
                            narg = arg + 2;
                            while (narg[0] != L'~') {
                                if (narg[0] < L'0' || narg[0] > L'9') ThrowMsg("Invalid argument 2");
                                sub_process *= 10;
                                sub_process += narg[0] - L'0';
                                ++narg;
                            }
                            for (size_t i = 1; narg[i] != L'~'; i++){
                                if (narg[i] == L'\0') ThrowMsg("Invalid argument 3");
                                sdir.push_back(narg[i]);
                            }
                            Console::subdir = new const wchar_t[wcslen(sdir.c_str())+1]{0};
                            wcscpy((wchar_t*)Console::subdir, sdir.c_str());
                            ++j; --i; --Console::argc;
                            break;
                        case L'\\':
                            ThrowMsg("IDK Argument");
                            break;
                        case L'#':
                            fixmintty = false;
                            ++j; --i; --Console::argc;
                            break;
                        default:
                            Console::argv[i] = arg;
                            break;
                    }
                } else Console::argv[i] = arg;
            }
            Console::out << "Argc: " << Console::argc << std::endl;

            auto _temp = realloc(Console::argv, sizeof(wchar_t*) * Console::argc);
            if (_temp) Console::argv = (const wchar_t**)_temp;
            else exit(0x73);

            wchar_t* tmpappdata = _wgetenv(L"APPDATA");
            if (!tmpappdata) ThrowMsg("\"APPDATA\" env variable not set");
            Console::user_data = tmpappdata;
            user_data.append(L"\\.factoryrush\\");

            wchar_t* tmptmpdata = _wgetenv(L"TEMP");
            if (!tmptmpdata) ThrowMsg("\"TEMP\" env variable not set");
            Console::tmp_data = tmptmpdata;
            tmp_data.append(L"\\.factoryrush\\");

            wchar_t* tmpdevdata = _wgetenv(L"ProgramData");
            if (!tmpdevdata) ThrowMsg("\"ProgramData\" env variable not set");
            Console::dev_data = tmpdevdata;
            dev_data.append(L"\\Factoryrush\\");

            Console::pid = _getpid();
            Console::window = GetHwnd();
            Console::terminal_name = GetWindowExecutableName(Console::window); // for checking the type of terminal

            setlocale(LC_ALL, "");

            if (fixmintty) goto dontstartthings;

            {
                auto input_thread_arg = new __getinput_arg{Console::input_buf, SLEEP_THREAD_INPUT};
                HANDLE input_thread = CreateThread(NULL, 0, InputThread, input_thread_arg, 0, NULL);
                Console::thread_handles->push_back(input_thread);
                *Console::super_thread_run = true;
                auto super_thread_arg = new __superthread_arg{Console::super_thread_run, Console::thread_handles};
                Console::super_thread = CreateThread(NULL, 0, Console::SuperThread, super_thread_arg, 0, NULL);
            }

            if (auto conpid = _wgetenv(L"ConEmuServerPID")) {
                DWORD cpid = conemupid = _wtol(conpid);
                Console::terminal_name = GetProcessExecutableName(cpid);
                auto GetProcExe = [](DWORD processId) {
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
                    if (processHandle) {
                        wchar_t exePath[MAX_PATH];
                        if (GetModuleFileNameEx(processHandle, NULL, exePath, MAX_PATH)) {
                            CloseHandle(processHandle);
                            return wstring(exePath);
                        }
                        CloseHandle(processHandle);
                    }
                    return wstring();
                };
                DWORD tabpid = Console::pid;
                if (mintty) {
                    DWORD pidfound = 0;
                    bool found = false;
                    auto childProcessIds = GetChildProcessIds(cpid);
                        for (auto&& childProcessId : childProcessIds) {
                            std::wstring childExeName = GetProcExe(childProcessId);
                            if (!childExeName.empty() && childExeName.substr(childExeName.size()-12) == L"\\conhost.exe") {
                                pidfound = childProcessId;
                                if (found) break;
                            } else if (!childExeName.empty() && childExeName.substr(childExeName.size()-11) == L"\\mintty.exe") {
                                cpid = childProcessId;
                                found = true;
                            }
                        }
                    if (found) {
                        if (pidfound) {
                        foundpid:
                            tabpid = pidfound;
                        } else {
                            found = false;
                            auto childMinttyProcessIds = GetChildProcessIds(cpid);
                                for (auto&& childProcessId : childMinttyProcessIds) {
                                    std::wstring childExeName = GetProcExe(childProcessId);
                                    if (!childExeName.empty() && childExeName.substr(childExeName.size()-12) == L"\\conhost.exe") {
                                        pidfound = childProcessId;
                                        break;
                                    }
                                    if (!childExeName.empty() && childExeName.substr(childExeName.size()-11) == L"\\mintty.exe") {
                                        cpid = childProcessId;
                                        found = true;
                                    }
                                }
                            if (pidfound)
                                goto foundpid;
                            if (found) {
                                auto childMinttyMinttyProcessIds = GetChildProcessIds(cpid);
                                for (auto childProcessId : childMinttyMinttyProcessIds) {
                                    std::wstring childExeName = GetProcExe(childProcessId);
                                    if (!childExeName.empty() && childExeName.substr(childExeName.size()-12) == L"\\conhost.exe") {
                                        pidfound = childProcessId;
                                        break;
                                    }
                                }
                                if (pidfound)
                                    goto foundpid;
                            }
                        }
                    } else mintty = false;
                }
                __conemuhandletabs_arg* tabarg = new __conemuhandletabs_arg();
                tabarg->pid = tabpid; tabarg->focused = &Console::tabactive;

                HANDLE TabCtrlThread = CreateThread(NULL, 0, ConemuHandleTabs, tabarg, 0, NULL);
                Console::thread_handles->push_back(TabCtrlThread);
                Console::out << L"Found pid: " << tabpid << L'\n' << L'\t' << flush;
                cerr << "Found pid: " << tabpid << endl;
            }

            if (mintty || conhost || wt) Console::parent_window = Console::window;
            if (wchar_t* conemuhwnd = _wgetenv(L"ConEmuHWND")) {

                if (conemuhwnd == nullptr) conemu = false;
                else {
                    ++conemuhwnd; ++conemuhwnd; // skip the first two characters `0x`
                    static_assert(sizeof(HWND) == sizeof(size_t), "HWND is not the same size as `size_t`");
                    switch (sizeof(HWND)) {
                    case sizeof(long):
                        Console::parent_window = bit_cast<HWND>(static_cast<size_t>(wcstoul(conemuhwnd,NULL,16)));
                        break;
                    case sizeof(long long):
                        Console::parent_window = bit_cast<HWND>(static_cast<size_t>(wcstoull(conemuhwnd,NULL,16)));
                        break;
                    default:
                        cerr << "Unacceptable HWND size: " << sizeof(HWND) << endl;
                        exit(0xE9);
                    }
                    //cerr << "Conemuhwnd: " << std::hex << Console::parent_window << std::dec << endl;
                }
                if (!conemu || cmder) {
                    conemu = cmder = true;
                    Console::window = Console::parent_window;
                    good = true;
                }
            }

        dontstartthings:
            LPSECURITY_ATTRIBUTES sec_atrs = new SECURITY_ATTRIBUTES();
            sec_atrs->nLength = sizeof(sec_atrs);
            sec_atrs->lpSecurityDescriptor = nullptr;
            sec_atrs->bInheritHandle = false;

            if (Console::sub_proc) {
                FILE* fl = _wfopen((tmp_data+subdir+L"window.dat").c_str(), L"w");
                if (Console::parent_window) fwrite(&Console::parent_window, sizeof(HWND), 1, fl);
                else fwrite(&Console::window, sizeof(HWND), 1, fl);
                fclose(fl);
                fl = _wfopen((tmp_data+subdir+L"pid.dat").c_str(), L"w");
                fwrite(&Console::pid, sizeof(Console::pid), 1, fl);
                fclose(fl);
                goto subdirset;
            }
            
            if (System::MakeDirectory(tmp_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + tmp_data + L"\"");

            if (System::MakeDirectory(user_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + user_data + L"\"");

            if (System::MakeDirectory(dev_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + dev_data + L"\"");

            if (System::MakeDirectory((dev_data+L"logs").c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (dev_data+L"logs") + L"\"");

            Console::subdir = new const wchar_t[to_wstring(Console::pid).size() + 2]{0};
            wcscpy((wchar_t*)Console::subdir, to_wstring(Console::pid).c_str());
            ((wchar_t*)Console::subdir)[to_wstring(Console::pid).size()] = L'\\';

            if (fixmintty) {
                wstring cmdpath = tmp_data + subdir;
                cmdpath.pop_back();
                auto ps1path = cmdpath + L".ps1";
                cmdpath.append(L".cmd");
                FILE* file = _wfopen(ps1path.c_str(), L"w");
                _setmode(_fileno(file),  _O_WTEXT); wint_t x = 0xfeff; fwrite(&x, sizeof(wint_t), 1, file);
                fwrite(L"& ", sizeof(wchar_t), 2, file);
                for (int i = 0; i < argc; i++)
                    fwprintf(file, L"\"%ls\" ", argv[i]);
                fwrite(L"\"\033#\"\nexit $LASTEXITCODE\n", sizeof(wchar_t), 24, file);
                fclose(file);

                file = _wfopen(cmdpath.c_str(), L"w");
                fwrite("@echo off\npowershell -NoProfile -ExecutionPolicy Bypass -Command \"%~dp0", sizeof(char), 71, file);
                fwrite(to_string(Console::pid).c_str(), sizeof(char), to_string(Console::pid).size(), file);
                fwrite(".ps1\nexit %ERRORLEVEL%\n", sizeof(char), 23, file);
                fclose(file);

                auto ret = System::RunProgram(cmdpath.c_str(), nullptr, nullptr);
                System::RemoveFile(cmdpath.c_str());
                System::RemoveFile(ps1path.c_str());
                exit(ret);
            }
            
        subdirset:

            if (System::MakeDirectory((tmp_data+subdir).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (tmp_data+subdir) + L"\"");

            if (System::MakeDirectory((tmp_data+subdir+L"proc").c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (tmp_data+subdir+L"proc") + L"\"");
            
            delete sec_atrs;

            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            wstringstream ss; ss << dev_data << L"logs\\";
            if (Console::sub_proc) ss << sub_process << L'\\';
            ss << std::put_time(&tm, L"%Y-%m-%d_%H\'%M\'%S");
            unsigned int filenum = 0;
            auto sstr = ss.str();
        logfile:
            filesystem::path out_file = filesystem::path(sstr + L".log");
            HANDLE logfl = CreateFile(out_file.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (logfl == (void*)-1) {
                auto err = GetLastError();
                if (err == ERROR_ALREADY_EXISTS) {
                    sstr = ss.str() + L"-" + to_wstring(++filenum);
                    goto logfile;
                }
                Console::out << L"Couldn't create log file: " << out_file.native() << L", with error: " << err << '\n';
                exit(0xC2);
            }
            CloseHandle(logfl);
            Console::real_out.open(out_file,ios::out);
            if (!Console::real_out.is_open()) {
                wcerr << L"Couldn't open log file: " << out_file.native() << L", with error: " << GetLastError() << '\n'; 
                exit(0xC1);
            }

            Console::screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            SetConsoleActiveScreenBuffer(Console::screen);
            
            Console::fd = GetStdHandle(STD_INPUT_HANDLE);
            GetConsoleMode(Console::fd, &Console::old_console);
            SetConsoleMode(Console::fd, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

            Console::device = GetDC(NULL);
            //Console::xyoffset = Console::GetXYCharOffset();

            Console::old_small_icon = reinterpret_cast<HICON>(SendMessage(Console::window, WM_GETICON, ICON_SMALL, 0));
            Console::old_big_icon = reinterpret_cast<HICON>(SendMessage(Console::window, WM_GETICON, ICON_BIG, 0));

        #ifdef _MSC_VER
            Console::new_icon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(NULL), L"MAINICON", IMAGE_ICON, 0, 0, 0));
        #else
            Console::new_icon = reinterpret_cast<HICON>(LoadImage(NULL, (System::GetRootDir() + L"\\assets\\images\\icon.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
        #endif

            SendMessage(Console::window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(Console::new_icon));
            SendMessage(Console::window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(Console::new_icon));

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(Console::screen, &csbi);
            auto val = AtrValToColors(csbi.wAttributes);
            Console::default_fcol = val.first;
            Console::default_bcol = val.second;

            GetConsoleCursorInfo(Console::screen, &Console::old_curinf);
            Console::cursor_size = old_curinf.dwSize;
            Console::cursor_visible = old_curinf.bVisible;
            
            Console::mouse_status.x = 0;
            Console::mouse_status.y = 0;
            
            initialised = true;

            atexit(Console::Fin);
            at_quick_exit(Console::Fin);

            signal(SIGINT, quick_exit);
            signal(SIGILL, quick_exit);
            signal(SIGABRT, quick_exit);
            signal(SIGFPE, quick_exit);
            signal(SIGSEGV, quick_exit);
            signal(SIGTERM, quick_exit);
            signal(SIGBREAK, quick_exit);

            if (sub_process) {
                Console::ret = sub(sub_process);
                Console::out << L"Console::ret: " << Console::ret; Console::out_endl();
                Console::Fin();
                exit(0);
            } else if (sub_proc) {
                Console::ret = Main();
                Console::Fin();
                exit(0);
            }
        }
    }

    void Console::Fin(void) {
        if (initialised) {
            *Console::super_thread_run = false;
            WaitForSingleObject(Console::super_thread, INFINITE);
            CloseHandle(Console::super_thread);

            for (size_t i = 0; i < Console::popup_pids.size(); i++) {
                const auto proc = OpenProcess(PROCESS_TERMINATE, false, popup_pids[i]);
                TerminateProcess(proc, 1);
                CloseHandle(proc);
            }

            if (!Console::sub_proc) {
                System::ClearDirectory((Console::tmp_data+Console::subdir).c_str());
                System::DeleteDirectory((Console::tmp_data+Console::subdir).c_str());
            } else {
                FILE* fl;
                Console::out << "Console::ret: " << Console::ret; Console::out_endl();
                fl = _wfopen((Console::tmp_data+Console::subdir+L"exit.dat").c_str(), L"w");
                fwrite(&Console::ret, sizeof(Console::ret), 1, fl);
                fclose(fl);
                if (!System::IsFile((Console::tmp_data+Console::subdir+L"result.dat").c_str()))
                    Console::SetResult(nullptr);
            }

            delete[] Console::subdir;

            Console::SetTitle(L"");

            HICON term_icon = nullptr;
            Console::out << "Console::old_small_icon: " << Console::old_small_icon; Console::out_endl();
            Console::out << "Console::old_big_icon: " << Console::old_big_icon; Console::out_endl();
            if (!Console::old_small_icon || !Console::old_big_icon) {
                auto term = Console::GetTerminalExecutableName();
                term_icon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(term.c_str()), L"MAINICON", IMAGE_ICON, 0, 0, 0));
                if (!term_icon) cerr << "Couldn't load icon" << endl;
            }

            if (Console::old_small_icon) SendMessage(Console::window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(Console::old_small_icon));
            else SendMessage(Console::window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(term_icon));
            if (Console::old_big_icon) SendMessage(Console::window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(Console::old_big_icon));
            else SendMessage(Console::window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(term_icon));
            DestroyIcon(Console::new_icon);

            SetConsoleCursorInfo(Console::screen, &Console::old_curinf);

            SetConsoleMode(Console::fd, Console::old_console);

            ReleaseDC(0, device);
            CloseHandle(Console::screen);

            Console::real_out.close();

            initialised = false;
        }
    }

    void cpp::Console::SetResult(const wchar_t* result) {
        FILE* fl = _wfopen((Console::tmp_data+subdir+L"result.dat").c_str(), L"w");
        size_t len = result ? wcslen(result) : 0;
        fwrite(&len, sizeof(size_t), 1, fl);
        if (len) fwrite(result, sizeof(wchar_t), len, fl);
        fclose(fl);
    }

    int16_t Console::GetWindowWidth(void) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(screen, &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    int16_t Console::GetWindowHeight(void) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(screen, &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    DWORD WINAPI WaitClrScrBuf(
#ifdef __GNUC__
        __attribute__((unused))
#endif
        LPVOID lpParam) {
        ::Sleep(1000);
        Console::ClearScreenBuffer();
        return 0;
    }

    void Console::FillScreenForce(const vector<vector<Symbol> >& symbols) {
        try {
            Symbol empty_sym = Symbol(L' ', 16, 16);
            const size_t win_width = GetWindowWidth(), win_height = GetWindowHeight(), height = symbols.size();
            if (win_width == 0 || win_height == 0) return;
    
            wchar_t* screen = new wchar_t[win_height*win_width];
            WORD* attributes = new WORD[win_height*win_width];
    
            for (size_t i = 0; i < win_height; i++) {
                if (i < height) {
                    const size_t width = symbols[i].size();
                    for (size_t j = 0; j < win_width; j++) {
                        if (i >= height || j >= width) {
                            screen[ i*win_width + j ] =  empty_sym.character;
                            attributes[ i*win_width + j ] = empty_sym.GetAttribute();
                            continue;
                        }
                        screen[ i*win_width + j ] = symbols[i][j].character;
                        attributes[ i*win_width + j ] = symbols[i][j].GetAttribute();
                    }
                } else {
                    for (size_t j = 0; j < win_width; j++) {
                        screen[ i*win_width + j ] =  empty_sym.character;
                        attributes[ i*win_width + j ] = empty_sym.GetAttribute();
                    }
                }
            }
    
            array<DWORD,2> written;
            BOOL out = WriteConsoleOutputCharacter(Console::screen, screen, win_width*win_height, { 0,0 }, &(written[0]) );
            if (out == 0) { exit(GetLastError()); }
            out = WriteConsoleOutputAttribute(Console::screen, attributes, win_width*win_height, { 0,0 }, &(written[1]) );
            if (out == 0) { exit(GetLastError()); }
            
            delete[] screen;
            delete[] attributes;
    
            CONSOLE_CURSOR_INFO cci;
            cci.bVisible = Console::cursor_visible;
            cci.dwSize = Console::cursor_size;
            SetConsoleCursorInfo(Console::screen, &cci);
            } catch (exception e) {
                cout << e.what() << endl;
            }
    }

    void Console::FillScreen(const vector<vector<Symbol> >& symbols) {
        if (!Console::screen_lock.try_lock()) return; // it's a slow function so we can't put multiple in a queue
        try {
        Symbol empty_sym = Symbol(L' ', 16, 16);
        const size_t win_width = GetWindowWidth(), win_height = GetWindowHeight(), height = symbols.size();
        if (win_width == 0 || win_height == 0) return;

        bool resize;
        if ((resize = ((int16_t)win_width != Console::old_scr_size.first || (int16_t)win_height != Console::old_scr_size.second)) || Console::refresh_screen) {
            Console::out << L"Resize: " << resize << L'\n';
            Console::out_flush();
            Console::old_symbols.clear();
            Console::FillScreenForce(symbols);
            if (Console::refresh_screen)
                Console::refresh_screen = false;
            if (resize) {
                HANDLE hThread = CreateThread(NULL,0,WaitClrScrBuf,NULL,0,NULL);
                thread_handles->push_back(hThread);
            }
            old_scr_size = {win_width,win_height};
            old_symbols = symbols;
            Console::screen_lock.unlock();
            return;
        }

        wstring now_screen; vector<WORD> now_attrs;
        COORD scr_lastcoord = {-1,-1}, atr_lastcoord = {-1,-1};
        bool scr_lastmatch = true, atr_lastmatch = true;

        array<DWORD,2> written;

		for (size_t i = 0; i < win_height; i++) {
            if (i < Console::old_symbols.size()) {
                if (i >= symbols.size())
                    for (size_t j = 0; j < win_width; j++) {
                        if (j >= Console::old_symbols[i].size()) {
                            if (!scr_lastmatch) {
                                //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                                if (out == 0) { Console::out << GetLastError() << endl; return; }
                                now_screen.clear();
                                scr_lastmatch = true;
                            }
                            if (!atr_lastmatch) {
                                //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                                if (out == 0) { Console::out << GetLastError() << endl; return; }
                                now_attrs.clear();
                                atr_lastmatch = true;
                            }
                            break;
                        }
                        if (Console::old_symbols[i][j].character != L' ') {
                            if (scr_lastmatch) { scr_lastcoord = {(SHORT)j,(SHORT)i}; }
                            scr_lastmatch = false;
                            now_screen.push_back(L' ');
                        } else if (!scr_lastmatch) {
                            //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                            BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                            if (out == 0) { Console::out << GetLastError() << endl; return; }
                            now_screen.clear();
                            scr_lastmatch = true;
                        }
                        if (Console::old_symbols[i][j].foreground != 16 || Console::old_symbols[i][j].background != 16) {
                            if (atr_lastmatch) { atr_lastcoord = {(SHORT)j,(SHORT)i}; }
                            atr_lastmatch = false;
                            now_attrs.push_back(Console::GenerateAtrVal(Console::old_symbols[i][j].foreground,Console::old_symbols[i][j].background));
                        } else if (!atr_lastmatch) {
                            //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                            BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                            if (out == 0) { Console::out << GetLastError() << endl; return; }
                            now_attrs.clear();
                            atr_lastmatch = true;
                        }
                    }
                else {
                    for (size_t j = 0; j < win_width; j++) {
                        if (j >= symbols[i].size())
                            for (;j < win_width; ++j) {
                                if (j >= Console::old_symbols[i].size()) {
                                    if (!scr_lastmatch) {
                                        //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                        BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                                        if (out == 0) { Console::out << GetLastError() << endl; return; }
                                        now_screen.clear();
                                        scr_lastmatch = true;
                                    }
                                    if (!atr_lastmatch) {
                                        //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                        BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                                        if (out == 0) { Console::out << GetLastError() << endl; return; }
                                        now_attrs.clear();
                                        atr_lastmatch = true;
                                    }
                                    break;
                                }
                                if (Console::old_symbols[i][j].character != L' ') {
                                    if (scr_lastmatch) { scr_lastcoord = {(SHORT)j,(SHORT)i}; }
                                    scr_lastmatch = false;
                                    now_screen.push_back(L' ');
                                } else if (!scr_lastmatch) {
                                    //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                    BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                                    if (out == 0) { Console::out << GetLastError() << endl; return; }
                                    now_screen.clear();
                                    scr_lastmatch = true;
                                }
                                if (Console::old_symbols[i][j].foreground != 16 || Console::old_symbols[i][j].background != 16) {
                                    if (atr_lastmatch) { atr_lastcoord = {(SHORT)j,(SHORT)i}; }
                                    atr_lastmatch = false;
                                    now_attrs.push_back(Console::GenerateAtrVal(Console::old_symbols[i][j].foreground,Console::old_symbols[i][j].background));
                                } else if (!atr_lastmatch) {
                                    //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                    BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                                    if (out == 0) { Console::out << GetLastError() << endl; return; }
                                    now_attrs.clear();
                                    atr_lastmatch = true;
                                }
                            }
                        else {
                            if ((j >= Console::old_symbols[i].size() && symbols[i][j].character != L' ') || (j < Console::old_symbols[i].size() && Console::old_symbols[i][j].character != symbols[i][j].character)) {
                                if (scr_lastmatch) { scr_lastcoord = {(SHORT)j,(SHORT)i}; }
                                scr_lastmatch = false;
                                now_screen.push_back(symbols[i][j].character);
                            } else if (!scr_lastmatch) {
                                //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                                if (out == 0) { Console::out << GetLastError() << endl; return; }
                                now_screen.clear();
                                scr_lastmatch = true;
                            }
                            if ((j >= Console::old_symbols[i].size() && (symbols[i][j].foreground != 16 || symbols[i][j].background != 16)) || (j < Console::old_symbols[i].size() && (Console::old_symbols[i][j].foreground != symbols[i][j].foreground || Console::old_symbols[i][j].background != symbols[i][j].background))) {
                                if (atr_lastmatch) { atr_lastcoord = {(SHORT)j,(SHORT)i}; }
                                atr_lastmatch = false;
                                now_attrs.push_back(Console::GenerateAtrVal(symbols[i][j].foreground,symbols[i][j].background));
                            } else if (!atr_lastmatch) {
                                //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                                BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                                if (out == 0) { Console::out << GetLastError() << endl; return; }
                                now_attrs.clear();
                                atr_lastmatch = true;
                            }
                        }
                    }
                }
                continue;
            }
            if (i < height) {
                const size_t width = symbols[i].size();
                for (size_t j = 0; j < win_width; j++) {
                    if (j >= width) {
                        if (!scr_lastmatch) {
                            //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                            BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                            if (out == 0) { Console::out << GetLastError() << endl; return; }
                            now_screen.clear();
                            scr_lastmatch = true;
                        }
                        if (!atr_lastmatch) {
                            //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                            BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                            if (out == 0) { Console::out << GetLastError() << endl; return; }
                            now_attrs.clear();
                            atr_lastmatch = true;
                        }
                        break;
                    }
                    if (symbols[i][j].character != L' ') {
                        if (scr_lastmatch) { scr_lastcoord = {(SHORT)j,(SHORT)i}; }
                        scr_lastmatch = false;
                        now_screen.push_back(symbols[i][j].character);
                    } else if (!scr_lastmatch) {
                        //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                        BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
                        if (out == 0) { Console::out << GetLastError() << endl; return; }
                        now_screen.clear();
                        scr_lastmatch = true;
                    }
                    if (!(symbols[i][j].background == 16 && symbols[i][j].foreground == 16)) {
                        if (atr_lastmatch) { atr_lastcoord = {(SHORT)j,(SHORT)i}; }
                        atr_lastmatch = false;
                        now_attrs.push_back(symbols[i][j].GetAttribute());
                    } else if (!atr_lastmatch) {
                        //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
                        BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
                        if (out == 0) { Console::out << GetLastError() << endl; return; }
                        now_attrs.clear();
                        atr_lastmatch = true;
                    }
                }
            } else break;
		}

        if (!scr_lastmatch) {
            //wcerr << L"WriteConsoleOutputCharacter: " << now_screen.size() << L' ' << L'+' << L' ' << L'{' << scr_lastcoord.X << L' ' << scr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
            BOOL out = WriteConsoleOutputCharacter(Console::screen, now_screen.c_str(), now_screen.size(), scr_lastcoord, &(written[0]) );
            if (out == 0) { Console::out << GetLastError() << endl; return; }
        }
        if (!atr_lastmatch) {
            //wcerr << L"WriteConsoleOutputAttribute: " << now_attrs.size() << L' ' << L'+' << L' ' << L'{' << atr_lastcoord.X << L' ' << atr_lastcoord.Y << L'}' << ' ' << L':' << L' ' << win_width << ' ' << win_height << endl;
            BOOL out = WriteConsoleOutputAttribute(Console::screen, now_attrs.data(), now_attrs.size(), atr_lastcoord, &(written[1]) );
            if (out == 0) { Console::out << GetLastError() << endl; return; }
        }

        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = Console::cursor_visible;
        cci.dwSize = Console::cursor_size;
        SetConsoleCursorInfo(Console::screen, &cci);

        old_scr_size = {win_width,win_height};
        old_symbols = symbols;

        } catch (exception e) {
            cerr << e.what() << endl;
            exit(1);
        }
        Console::screen_lock.unlock();
    }

    void Console::HandleKeyboard(void) {

        // toggles
        Console::keys_toggled.ScrollLock = GetKeyState(VK_SCROLL) & 1;
        Console::keys_toggled.NumLock = GetKeyState(VK_NUMLOCK) & 1;
        Console::keys_toggled.CapsLock = GetKeyState(VK_CAPITAL) & 1;

        // press - release
        key_hit = -1;
        key_released = -1;
        for (int i = 0; i < KEYBOARD_MAX; i++) {
            SHORT keyState = GetKeyState(i);
            if (!key_states[i] && keyState & 0x8000) key_hit = i;
            if (key_states[i] && !(keyState & 0x8000)) key_released = i;
            
            key_states[i] = keyState & 0x8000;
        }
    }

    void Console::HandleMouseAndFocus() {
        if (mintty && !conemu && !good) {
            auto file = Console::GetTerminalExecutableName();
            if (file.find(L"mintty") == std::wstring::npos) {
                Console::window = GetForegroundWindow();
            }
            file = Console::GetTerminalExecutableName();
            good = file.find(L"mintty") != std::wstring::npos;
        }

        auto old_focus = Console::focused;

        INPUT_RECORD record;
        DWORD evnts=0,numRead;
        
        Console::this_mouse_button = -1;
        Console::this_mouse_down = false;
        Console::this_mouse_combo = 0;
        Console::mouse_buttons_down[MOUSE_SCROLL_UP] = false;
        Console::mouse_buttons_down[MOUSE_SCROLL_DOWN] = false;
        Console::mouse_status.scroll = {false,false};
        GetNumberOfConsoleInputEvents(Console::fd, &evnts);
        auto oldmouse = Console::mouse_buttons_down;
        if (evnts == 0) goto getinputx;
        for (unsigned i = 0; i < evnts; ++i) {
        
            if(!ReadConsoleInput(Console::fd, &record, 1, &numRead)) {
                Console::out << L"ReadConsoleInput";
                int err = GetLastError();
                exit(0x82);
            }
            bitset<5> event(record.EventType);
            if (event[0]) {
                // KEY_EVENT
            }

            if (event[1]) {
                // MOUSE_EVENT

                auto mouse = record.Event.MouseEvent;
                bitset<4> flags(mouse.dwEventFlags);

                if (mouse.dwEventFlags & MOUSE_MOVED) {
                    Console::mouse_status.x = mouse.dwMousePosition.X;
                    Console::mouse_status.y = mouse.dwMousePosition.Y;
                }
                Console::mouse_status.primary = mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;
                Console::mouse_status.secondary = mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED;
                Console::mouse_status.middle = mouse.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED;
                Console::mouse_buttons_down[6] = mouse.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED;
                Console::mouse_buttons_down[7] = mouse.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED;
                Console::mouse_status.scroll = { (mouse.dwEventFlags & MOUSE_WHEELED), (mouse.dwEventFlags & MOUSE_WHEELED && !(mouse.dwButtonState & 0xFF000000)) }; // weird number but work [not like the official documented way], it's even a bit opposite to the documented one
                Console::mouse_buttons_down[1] = mouse_status.primary;
                Console::mouse_buttons_down[2] = mouse_status.middle;
                Console::mouse_buttons_down[3] = mouse_status.secondary;
                Console::mouse_buttons_down[4] = mouse_status.scroll.second;
                Console::mouse_buttons_down[5] = mouse_status.scroll.first && !mouse_status.scroll.second;
                /*
                if (mouse_buttons_down[4]) {
                    cerr << "^";
                } else if (mouse_buttons_down[5]) {
                    cerr << "v";
                }//*/
                //if (mouse.dwButtonState & 0b11111) Console::out << L'0' << L'x' << std::hex << mouse.dwButtonState << L'\n';
                Console::this_mouse_combo = (flags[1] ? this_mouse_combo : 0) + 1;
                for (int i = 1; i <= 7; i++) if (mouse_buttons_down[i] && !oldmouse[i]) {
                    Console::this_mouse_down = true;
                    Console::this_mouse_combo = (mouse.dwEventFlags & DOUBLE_CLICK) ? 2 : 1;
                    Console::this_mouse_button = i;
                    break;
                }
                for (int i = 1; i <= 7; i++) if (!mouse_buttons_down[i] && oldmouse[i]) {
                    Console::this_mouse_down = false;
                    Console::this_mouse_button = i;
                    break;
                }
            }

            if (event[2]) {
                // WINDOW_BUFFER_SIZE_EVENT
            }

            if (event[3]) {
                // MENU_EVENT
            }

            if (event[4]) {
                // FOCUS_EVENT
                // reserved for some reason
                // I don't quacking care
                Console::focused = record.Event.FocusEvent.bSetFocus;
            }
        }
/*
        Console::focused = (Console::window == GetForegroundWindow());

        RECT rect;
        GetClientRect(Console::window,&rect);
        if (Console::auto_size_updates) {
            auto n_width = Console::GetWindowWidth();
            auto n_height = Console::GetWindowHeight();

            if (Console::old_width != n_width || Console::old_height != n_height || Console::old_rect.bottom - Console::old_rect.top != rect.bottom - rect.top || Console::old_rect.right - Console::old_rect.left != rect.right - rect.left) {
                auto offsiz = GetOffsetSymSize(7,14,15);
                Console::scr_offs = offsiz.first;
                Console::sym_size = offsiz.second;
                Console::old_width = n_width;
                Console::old_height = n_height;
                Console::old_rect = rect;
            }
        }
        
        POINT corner = {rect.left,rect.top}; ClientToScreen(Console::window,&corner);
        POINT mouse;
        GetCursorPos(&mouse);
        ScreenToClient(window,&mouse);
        mouse.x -= Console::scr_offs.first;
        mouse.y -= Console::scr_offs.second;
        mouse.x /= Console::sym_size.first;
        mouse.y /= Console::sym_size.second;
        Console::mouse_status.x = mouse.x;
        Console::mouse_status.y = mouse.y;
        Console::mouse_status.y = mouse.y;
*/
    getinputx:
        if (conemu && mintty)
            Console::focused = Console::parent_window == GetForegroundWindow();
        

        int bytes = Console::input_buf->size();
        if (bytes == 0) return;
        bytes += nstrlen(Console::buf);

        wchar_t wc;
        while (bytes > 0) {
            buf_it = 0;
            wc = GetChar(); --bytes;
            switch (wc) {
                case L'\033':
                    if (bytes == 0) return;
                    wc = GetChar(); --bytes;
                    switch (wc) {
                    case L'[':
                        if (bytes == 0) return;
                        wc = GetChar(); --bytes;
                        switch (wc) {
                            case L'O':
                            case L'I':
                                break;
                            case L'M':
                                if (bytes == 0) return;
                                wc = GetChar(); --bytes;
                                if (bytes == 0) return;
                                wc = GetChar(); --bytes;
                                if (bytes == 0) return;
                                wc = GetChar();
                                buf[0] = L'\0';
                                break;
                            case L'<':
                                while (--bytes > 0 && wc != L'M')
                                    wc = GetChar();
                                if (bytes == 0) return;
                                buf[0] = L'\0';
                                break;
                            default:
                                Console::PushChar('\033');
                                Console::PushChar('[');
                                Console::PushChar(wc);
                                buf[0] = L'\0';
                                break;
                        }
                        break;
                    default:
                        Console::PushChar('\033');
                        Console::PushChar(wc);
                        buf[0] = L'\0';
                        break;
                    }
                    break;
                default:
                    Console::PushChar(wc);
                    buf[0] = L'\0';
                    break;
            }
        }
        return;
    }

    void Console::HandleOutput(void) {
        auto str = Console::out.str();
        Console::out.clear();
        if (str.empty()) return;
        Console::out.str(wstring());
        Console::real_out.write(str.c_str(), str.size());
        Console::real_out.flush();
    }

    void Console::Beep(void) {
        if (conemu && !mintty) System::RunProgram0((System::GetRootDir() + L"\\bin\\beep.exe").c_str(), (const wchar_t*)nullptr); // Conemu blocks the Beep for some reason
        else MessageBeep(0xFFFFFFFF);
    }

    void SysSleep(int microseconds) {
        Sleep((int)(microseconds/1000));
    }

    bool Console::IsKeyDown(Key::Enum key) {
        return Console::key_states[static_cast<size_t>(key)];
    }

    Key::Enum cpp::Console::KeyTyped(void) {
        return Key::Enum::NONE;
    }

    Key::Enum Console::KeyPressed(void) {
        if (Console::key_hit < 0) return Key::Enum::NONE;
        return static_cast<Key::Enum>(Console::key_hit);
    }

    Key::Enum Console::KeyReleased(void) {
        if (Console::key_released < 0) return Key::Enum::NONE;
        return static_cast<Key::Enum>(Console::key_released);
    }

    uint8_t Console::Symbol::GetAttribute(void) const {
        return GenerateAtrVal(this->foreground,this->background);
    }

    void Console::Symbol::SetAttribute(uint8_t attribute) {
        this->foreground = AtrValToColors(attribute).first;
        this->background = AtrValToColors(attribute).second;
    }

    Console::Symbol::Symbol(uint8_t attribute) {
        this->character = L' ';
        this->SetAttribute(attribute);
    }

    
    mutex Console::screen_lock = {};
    tsqueue<wchar_t>* Console::input_buf = new tsqueue<wchar_t>();
    HANDLE Console::super_thread = HANDLE();
    atomic<bool>* Console::super_thread_run = new atomic<bool>();
    tsvector<HANDLE>* Console::thread_handles = new tsvector<HANDLE>();
    HANDLE Console::screen = HANDLE();
    HANDLE Console::fd = HANDLE();
    HWND Console::window = HWND();
    HWND Console::parent_window = HWND();
    HDC Console::device = HDC();
    HICON Console::old_small_icon = HICON();
    HICON Console::old_big_icon = HICON();
    HICON Console::new_icon = HICON();
    CONSOLE_CURSOR_INFO Console::old_curinf = CONSOLE_CURSOR_INFO();
    DWORD Console::old_console = DWORD();
    uint8_t Console::default_fcol = uint8_t();
    uint8_t Console::default_bcol = uint8_t();
    const wchar_t* Console::subdir = nullptr;
    wofstream Console::real_out = wofstream();
    //pair<uint16_t,uint16_t> Console::xyoffset = pair<uint16_t,uint16_t>();

#else
    // Not windows (Probably Posix and/or Unix)

    void Console::HandleOutput(void) {
        auto str = Console::out.str();
        Console::out.clear();
        if (str.empty()) return;
        Console::out.str(wstring());
        Console::real_out.write(str.c_str(), str.size());
        Console::real_out.flush();
    }
    
    void cpp::Console::SetResult(const char* result) {
        FILE* fl = fopen((Console::tmp_data+subdir+"result.dat").c_str(), "w");
        size_t len = result ? strlen(result) : 0;
        fwrite(&len, sizeof(size_t), 1, fl);
        if (len) fwrite(result, sizeof(char), strlen(result), fl);
        fclose(fl);
    }

    void cpp::Console::MoveCursor(int x, int y) {
        Console::cursorpos.first = x;
        Console::cursorpos.second = y;
        Console::EscSeqMoveCursor();
    }

    void cpp::Console::ShowCursor(void) {
        Console::cursor_visible = true;
        Console::EscSeqSetCursor();
    }

    void cpp::Console::HideCursor(void) {
        Console::cursor_visible = false;
        Console::EscSeqSetCursor();
    }

    void cpp::Console::SetCursorSize(uint8_t size) {
        Console::cursor_size = size;
        Console::EscSeqSetCursorSize();
    }

    void cpp::Console::SetTitle(const char_t* title) {
        EscSeqSetTitle(title);
    }

    void cpp::Console::Beep(void) {
        csimp::SoundSystem_PlaySound(uniconv::NativeStringToUnicode((System::GetRootDir() + sep + N("assets") + sep + N("illegal-operation.wav")).c_str()), 0);
    }

    mbstate_t Console::streammbs = mbstate_t();
    ofstream Console::real_out = ofstream();

#ifdef __linux__
// linux
    #include "System.hpp"

    inline clang_constexpr string GenerateEscapeSequence(uint8_t,uint8_t);

    inline constexpr int parse_input(int show_keycodes, const char * buf, int n) {
        int out = 0;
        //char buf[16];
        //int n = read(fd, buf, sizeof(buf));
            
            for (int i = 0; i < n; i++) {
                if (!show_keycodes) {
                    out *= KEYBOARD_MAX;
                    out += buf[i];
                } else {
                    out = buf[i] & 0x7f; // set keycode
                    out += buf[i] & 0x80 ? 0x100 : 0x000; // add bit 0 if press bit 1 if release
                }
            }
        return out;
    };

    int getparent(int pid) {
        int ppid;
        char buf[BUFSIZ*2];
        char procname[32];

        FILE *fl = fopen(string("/proc/" + to_string(pid) + "/status").c_str(), "r");
        size_t ret = fread(buf, sizeof(char), BUFSIZ*2-1, fl);
        if (!ret) return 0;
        buf[ret++] = '\0'; 
        fclose(fl);
        char *ppid_loc = strstr(buf, "\nPPid:");
        if (ppid_loc) {
            int ret = sscanf(ppid_loc, "\nPPid:%d", &ppid);
            if (!ret || ret == EOF) return 0;
            return ppid;
        }
        return 0;
    }

    inline string FindTerminalEmulator() {
        string term;
        return "to be implemented";
    }

    void Console::Init(void) {
        if (!initialised) {
            if (setuid(0) == -1)
                exit(0xf0);
            
            Console::emulator = !(getenv("DISPLAY") == nullptr);

            char path[256];
            size_t sizt = readlink("/proc/self/exe" , path, 256);
            
            if (Console::emulator) {
                fwrite("\033[?1049h", sizeof(char), 8, stderr);
            }

            int sub_process = 0;

            FILE *cmdline = fopen("/proc/self/cmdline", "rb");
            char *arg = 0; size_t size = 0;
            int out = getdelim(&arg, &size, 0, cmdline);
            while(out != -1) {
                if (strlen(arg) > 1 && arg[0] == '\033') {
                    string sdir; char* narg;
                    switch (arg[1]) {
                    case '&':
                        // launched as popup
                        Console::sub_proc = true;
                        if (strlen(arg) < 3) exit(0x31);
                        sub_process = 0;
                        narg = arg + 2;
                        while (narg[0] != '~') {
                            if (narg[0] < '0' || narg[0] > '9') exit(0x32);
                            sub_process *= 10;
                            sub_process += narg[0] - '0';
                            ++narg;
                        }
                        for (size_t i = 1; narg[i] != '~'; i++){
                            if (narg[i] == '\0') exit(0x33);
                            sdir.push_back(narg[i]);
                        }
                        Console::subdir = new const char[strlen(sdir.c_str())+1]{0};
                        strcpy((char*)Console::subdir, sdir.c_str());
                        
                        out = getdelim(&arg, &size, 0, cmdline);
                        break;
                    case '+':
                        // launched for permissions
                        parent = true;
                        if (strlen(arg) < 6) exit(0x36);

                        // root type
                        narg = arg + 2;
                        while (narg[0] != '~') {
                            if (narg[0] < '0' || narg[0] > '9') exit(0x37);
                            root_type *= 10;
                            root_type += narg[0] - '0';
                            ++narg;
                        }

                        // parent pid
                        pid = 0;
                        ++narg;
                        while (narg[0] != '~') {
                            if (narg[0] < '0' || narg[0] > '9') exit(0x37);
                            pid *= 10;
                            pid += narg[0] - '0';
                            ++narg;
                        }
                        break;
                    default:
                        exit(0x3F);
                    }
                    out = getdelim(&arg, &size, 0, cmdline);
                } else {
                    Console::argv = (const char**)realloc(Console::argv,++Console::argc); Console::argv[Console::argc-1] = arg;
                    arg = 0; size = 0;
                    out = getdelim(&arg, &size, 0, cmdline);
                }
            }
            fclose(cmdline);

            struct stat st;
            //int parent_pid;
            //int cnt;
            Console::pid = parent ? pid : getpid();
            if (Console::sub_proc) goto subdirset;

            if (stat("/tmp/.factoryrush/", &st) == -1)
                mkdir("/tmp/.factoryrush/", 0777);
            
            /*
            parent_pid = getppid();
            cnt = 0;
    parentcheck:
            parent = (stat((string("/tmp/.factoryrush/") + to_string(parent_pid)).c_str(), &st) != -1);
            if (parent || cnt > 5) goto sureparent;
            cmdline = fopen(string("/proc/" + to_string(parent_pid) + "/cmdline").c_str(), "rb");
            out = getdelim(&arg, &size, 0, cmdline);
            fclose(cmdline);
            if ((arg[0] == 'd' && arg[1] == 'o' && arg[2] == 'a' && arg[3] == 's' && arg[4] == '\0') || (arg[0] == 's' && arg[1] == 'u' && arg[2] == 'd' && arg[3] == 'o' && arg[4] == '\0') || (arg[0] == 's' && arg[1] == 'u' && arg[2] == '\0') || (arg[0] == 's' && arg[1] == 'h' && arg[2] == '\0')) {
                parent_pid = getparent(parent_pid);
                ++cnt;
                goto parentcheck;
            }
    sureparent:
            Console::pid = parent ? parent_pid : getpid();
            */
            Console::subdir = new const char[to_string(Console::pid).size() + 2]{0};
            strcpy((char*)Console::subdir, to_string(Console::pid).c_str());
            ((char*)Console::subdir)[to_string(Console::pid).size()] = '/';
            
            if (!parent && (stat((string("/tmp/.factoryrush/") + subdir).c_str(), &st) == -1))
                mkdir((string("/tmp/.factoryrush/") + subdir).c_str(), 0777);
    subdirset:

            string initpth = (string("/tmp/.factoryrush/") + subdir + string("initialized.dat"));
            FILE *initfl = fopen(initpth.c_str(), "w");
            fprintf(initfl, "%d", 1);
            fclose(initfl);

            fd = getfd(0);
            if (!Console::emulator) {
                if (stat("/etc/init.d/gpm", &st) == -1) Console::no_gpm = true;
                else {
                    Console::no_gpm = System::Shell("/etc/init.d/gpm status | grep '(running)' > /dev/null");
                    if (!Console::no_gpm) System::RunProgram("/etc/init.d/gpm", "stop", nullptr);
                }

                fb_fd = open("/dev/fb0", O_RDONLY);
                if (fb_fd < 0) {
                    perror("Error opening /dev/fb0");
                    exit(0x71);
                }

                // initially in the middle of the screen
                fb_var_screeninfo vinfo;
                if (ioctl(Console::fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
                    perror("Error reading variable information");
                    exit(0x70);
                }
                Console::pixelpos.first = vinfo.xres/2;
                Console::pixelpos.second = vinfo.yres/2;
                ioctl(STDERR_FILENO, TIOCGWINSZ, &window_size);
                Console::mouse_status.x = window_size.ws_col/2;
                Console::mouse_status.y = window_size.ws_row/2;

                mouse_fd = getmousefd(nullptr);
            }

            if (fd < 0 || fb_fd < 0 || mouse_fd < 0) {
                if (sub_proc) exit(0x34);
                if (parent) exit(0x38);
                if (Console::root_type) exit(0x20);
                // now this shouldn't happen at all so maybe add this:
                //exit(0x17);

                string args_c;
                string doas_keepenv = path;
                while (doas_keepenv.back() != '/') doas_keepenv.pop_back();
                doas_keepenv.append("doas-keepenv.sh");
                args_c.append(System::Shell("sudo --help 2>&1 >/dev/null") == 127 ? System::Shell("doas 2>&1 >/dev/null") == 127 ? "su -c \"" : "doas-keepenv" : "sudo -E ");
                bool issu = args_c.size() == 2 && args_c[0] == 's' && args_c[1] == 'u';
                bool isdoas = args_c[0] == 'd' && args_c[1] == 'o';
                uint8_t sudolevel = issu ? 1 : isdoas ? 2 : 3; 

                fwrite("\033[H", sizeof(char), 3, stderr);
                fwrite("\033[J", sizeof(char), 3, stderr);

                for (int i = 0; i < Console::argc; i++)
                    args_c.append(Console::argv[i]);
                    args_c.push_back(' ');
                
                args_c.push_back('\033');
                args_c.push_back('+');
                args_c.append(to_string(sudolevel));
                args_c.push_back('\\');
                args_c.push_back('~');
                args_c.append(to_string(Console::pid));
                args_c.push_back('\\');
                args_c.push_back('~');

                if (issu) args_c.push_back('\"');

                initfl = fopen(initpth.c_str(), "w");
                fprintf(initfl, "%d", 0);
                fclose(initfl);

                auto log = (GenerateEscapeSequence(1,16) + "\nCouldn\'t get a file descriptor referring to the console." + GenerateEscapeSequence(16,16) + "\nTry logging in" + (issu ? "\n[su] " : "\n"));
                fwrite(log.c_str(), sizeof(char), log.size(), stderr);
                int code = System::Shell(args_c.c_str()); 

                initfl = fopen(initpth.c_str(), "r");
                int isinit = 1; 
                auto retx = fscanf(initfl, "%d", &isinit);
                fclose(initfl);

                remove((string("/tmp/.factoryrush/") + Console::subdir + "initialized.dat").c_str());
                remove((string("/tmp/.factoryrush/") + Console::subdir).c_str());

                if (!isinit) {
                    log = (GenerateEscapeSequence(1,16) + "Authentication failed" + GenerateEscapeSequence(16,16) + "\nPress any key to exit\n");
                    fwrite(log.c_str(), sizeof(char), log.size(), stderr);
                    tcgetattr(STDIN_FILENO,&old_termios);
                    termios term_ios = old_termios;
                    term_ios.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO, TCSANOW, &term_ios);
                    char x[1];
                    read(STDIN_FILENO, x, 1);
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
                    fwrite("\033[?1049l", sizeof(char), 8, stderr);
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
                //exit(code);

                //string error = GenerateEscapeSequence(1,16) + "\nCouldn't get a file descriptor referring to the console.\nCheck if you have acces to /dev/tty and /dev/console.\n" + GenerateEscapeSequence(4,16) + "\n\ttry: " + GenerateEscapeSequence(6,16) + command.c_str() + "\033[0m\n";
                //throw(runtime_error(error.c_str()));
            }

            if (ioctl(fd, KDGKBMODE, &old_kbdmode)) {
                throw("ioctl KDGKBMODE error");
            }

            close(fd);

            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode");
            char val[2] = { ('0' + K_MEDIUMRAW),'\0' };

            auto ret2 = System::RunProgram(command.c_str(),val,nullptr);
            auto old_kbdmode2 = ret2;
            if (ret2 < 0) {
                throw("setkbdmode error");
            }

            fd = getfd(0);
            if (fd < 0) {
                string error = GenerateEscapeSequence(1,16) + "\nCouldn't get a file descriptor referring to the console.\nCheck if you have acces to /dev/tty and /dev/console.\n" + GenerateEscapeSequence(4,16) + "\n\ttry: " + GenerateEscapeSequence(6,16) + command.c_str() + "\033[0m\n";
                throw(runtime_error(error.c_str()));
            }

            tcgetattr(STDIN_FILENO,&old_termios);
            termios term_ios = old_termios;
            term_ios.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &term_ios);

            tcgetattr(fd,&old_fdterm);
            term_ios = old_fdterm;
            term_ios.c_lflag &= ~(ICANON | ECHO | ISIG);
            term_ios.c_iflag = 0;
            term_ios.c_cc[VMIN] = 0xff;
            term_ios.c_cc[VTIME] = 1;
            tcsetattr(fd, TCSANOW, &term_ios);

            setlocale(LC_CTYPE, "UTF-8");

            if (Console::emulator) {
                Console::XtermInitTracking();
            }

            for (auto t = 0; t < MAX_NR_KEYMAPS; t++) {
                if (t > UCHAR_MAX) {
                    exit(18);
                }
                for (auto i = 0; i < NR_KEYS; i++) {
                    if (i > UCHAR_MAX) {
                        exit(18);
                    }

	                struct kbentry ke;
                    ke.kb_table = (unsigned char) t;
                    ke.kb_index = (unsigned char) i;
                    ke.kb_value = 0;

                    if (ioctl(fd, KDGKBENT, (unsigned long)&ke)) {
                        exit(19);
                    }

                    if (!i && ke.kb_value == K_NOSUCHMAP)
                        break;
                    
                    if (KTYP(ke.kb_value) == KT_LETTER)
                        ke.kb_value = K(KT_LATIN, KVAL(ke.kb_value));

                    if (ke.kb_value == UINT16_MAX) {
                        // ? idk mabe just push back UNDEFIEND
                        exit(20);
                    }
                    key_chart[t][i] = Key::Class::ToEnum(ke.kb_value);
                }
            }

            //
            
            /*HandleToggles once so next time we can see if they changed
            {
                char flags, leds;
                if (ioctl(Console::fd, KDGKBLED, &flags) == -1)
                    exit(12);
            
                if (ioctl(Console::fd, KDGETLED, &leds) == -1)
                    exit(11);
                

                short mode = (flags & 0x7);
                short light = (leds & 0x7);
                Console::keys_toggled.ScrollLock = mode & LED_SCR;

                // toggled keys get
                int capslock = getled("capslock") > 0 || mode & LED_CAP || light & LED_CAP;
                int scrolllock = getled("scrolllock") > 0 || mode & LED_SCR || light & LED_SCR;
                int numlock = getled("numlock") > 0 || mode & LED_NUM || light & LED_NUM;
                int micmute = getled("micmute") > 0; // unused for now

                Console::keys_toggled.ScrollLock = getled("scrolllock");
                Console::keys_toggled.NumLock = getled("numlock");
                Console::keys_toggled.CapsLock = getled("capslock");
            }//*/

            locale::global(locale(""));

            Console::terminal_name = FindTerminalEmulator();

            initialised = true;

            atexit(Fin);
            at_quick_exit(Fin);

            signal(SIGHUP, quick_exit);
            signal(SIGINT, quick_exit);
            signal(SIGQUIT, quick_exit);
            signal(SIGILL, quick_exit);
            signal(SIGTRAP, quick_exit);
            signal(SIGABRT, quick_exit);
            signal(SIGIOT, quick_exit);
            signal(SIGFPE, quick_exit);
            signal(SIGKILL, quick_exit);
            signal(SIGUSR1, quick_exit);
            signal(SIGSEGV, quick_exit);
            signal(SIGUSR2, quick_exit);
            signal(SIGPIPE, quick_exit);
            signal(SIGTERM, quick_exit);
        #ifdef SIGSTKFLT
            signal(SIGSTKFLT, quick_exit);
        #endif
            signal(SIGCHLD, quick_exit);
            signal(SIGCONT, quick_exit);
            signal(SIGSTOP, quick_exit);
            signal(SIGTSTP, quick_exit);
            signal(SIGTTIN, quick_exit);
            signal(SIGTTOU, quick_exit);

            if (sub_process) {
                Console::ret = sub(sub_process);
                Console::Fin();
                exit(ret);
            }
        }
    }

    void Console::Fin(void) {
        if (initialised) {

            for (size_t i = 0; i < Console::popup_pids.size(); i++)
                kill(Console::popup_pids[i], SIGTERM);
            
            // remove files
            if (Console::sub_proc) {
                FILE* fl;
                fl = fopen((Console::tmp_data+Console::subdir+"exit.dat").c_str(), "w");
                fwrite(&Console::ret, sizeof(Console::ret), 1, fl);
                fclose(fl);

                //if (!System::IsFile((Console::tmp_data+Console::subdir+L"result.dat").c_str()))
                    SetResult(nullptr);

                fl = fopen((string("/tmp/.factoryrush/") + Console::subdir + "initialized.dat").c_str(), "w");
                fwrite("-1", sizeof(char), 2, fl);
                fclose(fl);

            } else if (!Console::parent) {
                remove((string("/tmp/.factoryrush/") + Console::subdir + "initialized.dat").c_str());
                // if parent exists than it will remove it
                remove((string("/tmp/.factoryrush/") + Console::subdir).c_str());
            }

            delete[] Console::subdir;

            if (Console::emulator) {
                Console::XtermFinishTracking();
                Console::EscSeqRestoreCursor();
            } else {
                close(fb_fd);
                close(mouse_fd);
                if (!Console::no_gpm) System::RunProgram("/etc/init.d/gpm", "start", nullptr);
            }
            
            //ioctl(fd, KDSETMODE, old_kbdmode);
            tcsetattr(fd,TCSANOW,&old_fdterm);
            close(fd);

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);

            char path[256];
            auto length = readlink( "/proc/self/exe" , path, 256);
            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode");
            char val[2] = "0"; val[0] += old_kbdmode;
            auto ret2 = System::RunProgram(command.c_str(),val,nullptr);
            old_kbdmode = ret2;
            if (ret2 < 0) {
                throw("setkbdmode error");
            }

            for (int i = 0; i < argc; i++) free((void*)Console::argv[i]);
            Console::argv = (const char**)realloc(Console::argv,0);
            Console::argc = 0;

            if (!Console::emulator) {
                fwrite("\033[H", sizeof(char), 3, stderr); // clear screen because we don't have an alternate buffer
                fwrite("\033[J", sizeof(char), 3, stderr);
            }

            initialised = false;
        }
    }

    void Console::HandleMouseAndFocus(void) {

        if (Console::emulator)
            return XtermMouseAndFocus();
        
        Console::this_mouse_button = -1;
        Console::this_mouse_down = false;
        Console::this_mouse_combo = 0;
        Console::mouse_buttons_down[3] = false;
        Console::mouse_buttons_down[4] = false;
        Console::mouse_status.scroll = {false,false};
        // redirect cin to Console::in
        int bytes;
        ioctl(STDIN_FILENO, FIONREAD, &bytes);
        while (bytes > 0) {
            char c = getc(stdin);
            Console::PushChar(c);
            --bytes;
        }

        // mouse cursor emulation
        input_event ie;
        int x = 0;
        while (true) {
            auto len = read(Console::mouse_fd, &ie, sizeof(input_event));
            if (len == -1)
                break;
            ++x;
            //printf("EVENT1: TYPE=%d, CODE=%d, VALUE=%d\n", ie.type, ie.code, ie.value);
            if (ie.type == EV_SYN && (ie.code == SYN_REPORT || ie.code == SYN_MT_REPORT)) if (Console::discard_mouse) {Console::evnts_siz = 0; Console::discard_mouse = false;} else goto parsego;
            else if (ie.type == EV_SYN && ie.code == SYN_DROPPED) { Console::discard_mouse = true; Console::evnts_siz = 0; }
            else if (evnts_siz < 255) { events[evnts_siz] = ie; ++evnts_siz; }
            else goto parsego;
            continue;
        parsego:
            fb_var_screeninfo vinfo;
            if (ioctl(Console::fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
                perror("Error reading variable information");
                exit(0x72);
            }
            if (ioctl(STDERR_FILENO, TIOCGWINSZ, &window_size)) {
                perror("Error reading window size");
                exit(0x73);
            }
            int button = 0; bool mousedown = false;
            for (int i = 0; i < evnts_siz; i++) {
            //printf("EVENT2: TYPE=%d, CODE=%d, VALUE=%d\n", events[i].type, events[i].code, events[i].value);
                switch (events[i].type)
                {
                case EV_REL:
                    switch (events[i].code)
                    {
                    case REL_X:
                        if (events[i].value > 0 || static_cast<int64_t>(Console::pixelpos.first) > -events[i].value) Console::pixelpos.first += events[i].value;
                        Console::pixelpos.first = min(vinfo.xres, Console::pixelpos.first );
                        break;
                    
                    case REL_Y:
                        if (events[i].value > 0 || static_cast<int64_t>(Console::pixelpos.second) > -events[i].value) Console::pixelpos.second += events[i].value;
                        Console::pixelpos.second = min(vinfo.yres, Console::pixelpos.second );
                        break;
                    case REL_WHEEL:
                        Console::mouse_status.scroll = {true,events[i].value > 0};
                        button = 5-(events[i].value > 0);
                        mousedown = true;
                        break;
                    case REL_Z:
                    case REL_RX:
                    case REL_RY:
                    case REL_RZ:
                    case REL_HWHEEL:
                    case REL_DIAL:
                    case REL_WHEEL_HI_RES:
                        break;
                    default:
                        break;
                    }
                    break;
                
                case EV_KEY:
                    switch (events[i].code)
                    {
                #if (BTN_MOUSE != BTN_LEFT)
                    case BTN_MOUSE:
                #endif
                    case BTN_LEFT:
                        Console::mouse_status.primary = events[i].value;
                        button = 1;
                        mousedown = events[i].value;
                        break;
                    case BTN_RIGHT:
                        Console::mouse_status.secondary = events[i].value;
                        button = 3;
                        mousedown = events[i].value;
                        break;
                    case BTN_MIDDLE:
                        Console::mouse_status.middle = events[i].value;
                        button = 2;
                        mousedown = events[i].value;
                        break;
                    case BTN_SIDE:
                    case BTN_EXTRA:
                    case BTN_FORWARD:
                    case BTN_BACK:
                    case BTN_TASK:
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
            int width = window_size.ws_col;
            int height = window_size.ws_row;
            long double divx = vinfo.xres / width;
            long double divy = vinfo.yres / height;
            Console::mouse_status.x = pixelpos.first/divx;
            Console::mouse_status.y = pixelpos.second/divy;

            if (button) {
                chrono::time_point<chrono::high_resolution_clock> now = chrono::high_resolution_clock::now();
                uint8_t fullbutton = 0b000000;
                fullbutton |= 0b0000001 * button; // BBBB
                    //fullbutton |= 0b0010000 * IsKeyDown(Key::Enum::ShiftL)]; // Shift
                    //fullbutton |= 0b0100000 * ; // Meta
                    //fullbutton |= 0b1000000 * ; // Ctrl
                //fullbutton == 0bCMSBBBB

                if (mousedown) {
                    if (Console::last_mouse_button == fullbutton && now - Console::last_click_time <= chrono::milliseconds(Console::double_click_max)) {
                        if (Console::last_mouse_combo < UINT8_MAX) ++Console::last_mouse_combo;
                    } else {
                        Console::last_mouse_button = (button < 3 || button > 5) ? fullbutton : -1;
                        Console::last_mouse_combo = 1;
                    }
                    Console::last_click_time = now;
                }

                Console::this_mouse_combo = ( Console::last_click_time == now ) ? Console::last_mouse_combo : 1;
                Console::this_mouse_down = mousedown;
                Console::mouse_buttons_down[fullbutton] = this_mouse_down;
                Console::this_mouse_button = fullbutton;
            }
            evnts_siz = 0;
        }
    }

    void Console::HandleKeyboard(void) {

        char flags, leds;
        if (ioctl(Console::fd, KDGKBLED, &flags) == -1)
            exit(11);
        
        if (ioctl(Console::fd, KDGETLED, &leds) == -1)
            exit(12);

        // key presses and releases

        int bytes, parsed, len;
        char buf[1];

        key_hit = -1;
        key_released = -1;
        int old_hit = -1;

        ioctl(Console::fd, FIONREAD, &bytes);
        if (!bytes) goto HandleToggles;
    
    ReadKeyboardAction:
        len = read(Console::fd, buf, sizeof(buf)); bytes -= len;
        // TODO -> parse multi-byte sequences

        if (len <= 0) {
            if (len < 0) { fprintf(stderr, "\nread error: %d\n", errno); exit(15); } 
            goto HandleToggles;
        }

        //fprintf(stderr, "bytes: %d\n", bytes);

        parsed = parse_input(true,buf,len);

        //fprintf(stdout, "\n\n x:%x\nparsed: %d,rel:%d\n\told: %d\n\n", buf[0], parsed%KEYBOARD_MAX, parsed/KEYBOARD_MAX, (int)(key_states[parsed % KEYBOARD_MAX]));
        if ( (!key_states[parsed % KEYBOARD_MAX]) && (!(parsed / KEYBOARD_MAX)) ) key_hit = parsed % KEYBOARD_MAX;
        if ( key_states[parsed % KEYBOARD_MAX] && (parsed / KEYBOARD_MAX) ) key_released = parsed % KEYBOARD_MAX;
        key_states[parsed % KEYBOARD_MAX] = !(parsed / KEYBOARD_MAX);
        //fprintf(stderr, "keycode: %d\n", Console::key_hit);
        //fprintf(stderr, "rels: %d\n", Console::key_released);

        if (!Console::emulator && Console::key_hit != old_hit) {
            if (Console::key_chart[0][Console::key_hit] == Key::Enum::CAPS) { flags ^= LED_CAP; leds ^= LED_CAP; /*fprintf(stderr, "caps\n");*/ }
            if (Console::key_chart[0][Console::key_hit] == Key::Enum::CTRLLLOCK) { flags ^= LED_CAP; leds ^= LED_CAP; /*fprintf(stderr, "ctrlllock\n");*/ } // sometimes capslock (a bug bypass)
            if (Console::key_chart[0][Console::key_hit] == Key::Enum::NUM) { flags ^= LED_NUM; leds ^= LED_NUM; /*fprintf(stderr, "num\n");*/ }
            if (/*???*/ Console::key_chart[0][Console::key_hit] == Key::Enum::BARENUMLOCK ) { flags ^= LED_NUM; leds ^= LED_NUM; /*fprintf(stderr, "barenum\n");*/ } // idk waht this is but it has numlock in it's name
            if (Console::key_chart[0][Console::key_hit] == Key::Enum::HOLD) { flags ^= LED_SCR; leds ^= LED_SCR; /*fprintf(stderr, "scroll\n");*/ }
            if (Console::key_chart[0][Console::key_hit] == Key::Enum::CAPS || Console::key_chart[0][Console::key_hit] == Key::Enum::CTRLLLOCK || Console::key_chart[0][Console::key_hit] == Key::Enum::NUM || /*???*/ Console::key_chart[0][Console::key_hit] == Key::Enum::BARENUMLOCK || Console::key_chart[0][Console::key_hit] == Key::Enum::HOLD) {
                if (ioctl(Console::fd, KDSKBLED, flags) == -1) exit(13);
                if (ioctl(Console::fd, KDSETLED, leds) == -1) exit(13);
            }
        }
        old_hit = Console::key_hit;
        if (bytes > 0) goto ReadKeyboardAction;

    HandleToggles:

        short mode = (flags & 0x7);
        short light = (leds & 0x7);

        // toggled keys get
        int capslock = getled("capslock") > 0 || mode & LED_CAP || light & LED_CAP;
        int scrolllock = getled("scrolllock") > 0 || mode & LED_SCR || light & LED_SCR;
        int numlock = getled("numlock") > 0 || mode & LED_NUM || light & LED_NUM;
        int micmute = getled("micmute") > 0; // unused for now

        Console::keys_toggled.ScrollLock = scrolllock;
        Console::keys_toggled.NumLock = numlock;
        Console::keys_toggled.CapsLock = capslock;

        //if (Console::keys_toggled.CapsLock) fprintf(stderr, "caps lock\n");
        //if (Console::keys_toggled.NumLock) fprintf(stderr, "num lock\n");
        //if (Console::keys_toggled.ScrollLock) fprintf(stderr, "scroll lock\n");
        //if (micmute) fprintf(stderr, "mic mute\n");

        return;
    }

    uniconv::utfstr Console::GetTerminalExecutableName() {
        if (!emulator) return ""; // later maybe handle tmux & similar
    }

    bool Console::IsKeyDown(Key::Enum key) {
        for (auto i = 0; i < KEYBOARD_MAX; ++i)
            if (Console::key_states[i] && Console::key_chart[0][i] == key)
                return true;
        return false;
    }

    Key::Enum cpp::Console::KeyTyped(void) {
        return Key::Enum::NONE;
    }

    Key::Enum Console::KeyPressed(void) {
        if (Console::key_hit < 0) return Key::Enum::NONE;
        return Console::key_chart[0][Console::key_hit];
    }

    Key::Enum Console::KeyReleased(void) {
        if (Console::key_released < 0) return Key::Enum::NONE;
        return Console::key_chart[0][Console::key_released];
    }

    utfstr Console::GetTerminalExecutableName(void) {
        return Console::terminal_name;
    }

    struct termios Console::old_fdterm = termios();
    int Console::old_kbdmode = int();
    int Console::fd = int();
    int Console::fb_fd = int();
    int Console::mouse_fd = int();
    bool Console::discard_mouse = bool(false);
    bool Console::no_gpm = bool(true);
    bool Console::parent = bool(false);
    uint8_t Console::root_type = uint8_t(0);
    Key::Enum Console::key_chart[MAX_NR_KEYMAPS][KEYBOARD_MAX] = { { Key::Enum::NONE } };

    pair<uint32_t,uint32_t> Console::pixelpos = pair<uint32_t,uint32_t>(0,0);
    input_event Console::events[255] = {input_event()};
    uint8_t Console::evnts_siz = 0;

#elif __APPLE__
// macOS

    void Console::Init(void) {
        if (!initialised) {

            int sub_process = 0;
            auto& appargv = *_NSGetArgv();
            Console::argc = *_NSGetArgc();
            Console::argv = (const char**)malloc(sizeof(char*) * Console::argc);

            int j = 0;
            for (int i = 0; i < Console::argc; i++) {
                auto&& arg = appargv[i+j];
                if (strlen(arg) > 1 && arg[0] == '\033') {
                    string sdir; const char* narg;
                    switch (arg[1]) {
                        case '&':
                            // launched as popup
                            Console::out << L"launched as popup" << L'\n';
                            Console::sub_proc = true;
                            if (strlen(arg) < 3) exit(0x31);
                            sub_process = 0;
                            narg = arg + 2;
                            while (narg[0] != '~') {
                                if (narg[0] < '0' || narg[0] > '9') exit(0x32);
                                sub_process *= 10;
                                sub_process += narg[0] - '0';
                                ++narg;
                            }
                            for (size_t i = 1; narg[i] != '~'; i++){
                                if (narg[i] == '\0') exit(0x33);
                                sdir.push_back(narg[i]);
                            }
                            Console::subdir = new const char[strlen(sdir.c_str())+1]{0};
                            strcpy((char*)Console::subdir, sdir.c_str());
                            ++j; --i; --Console::argc;
                            break;
                        case '\\':
                            exit(0xE4);
                        default:
                            Console::argv[i] = arg;
                            break;
                    }
                } else
                    Console::argv[i] = arg;
            }

            auto _temp = realloc(Console::argv, sizeof(char*) * Console::argc);
            if (_temp) Console::argv = (const char**)_temp;
            else exit(0x73);


            struct stat st;
            Console::ppid = getppid();
            Console::pid = getpid();
            if (!Console::sub_proc) {
                if (stat("/tmp/.factoryrush/", &st) == -1)
                    mkdir("/tmp/.factoryrush/", 0777);
                
                Console::subdir = new const char[to_string(Console::pid).size() + 2]{0};
                strcpy((char*)Console::subdir, to_string(Console::pid).c_str());
                ((char*)Console::subdir)[to_string(Console::pid).size()] = '/';
                
                if (stat((string("/tmp/.factoryrush/") + subdir).c_str(), &st) == -1)
                    mkdir((string("/tmp/.factoryrush/") + subdir).c_str(), 0777);
            }
            
            string appdata = getenv("HOME");
            appdata.append("/.factoryrush/");

            if (stat(appdata.c_str(), &st) == -1)
                mkdir(appdata.c_str(), 0777);

            if (stat((string("/tmp/.factoryrush/") + subdir + "proc").c_str(), &st) == -1)
                mkdir((string("/tmp/.factoryrush/") + subdir + "proc").c_str(), 0777);

            if (stat((appdata + subdir).c_str(), &st) == -1)
                mkdir((appdata + subdir).c_str(), 0777);

            string initpth = (string("/tmp/.factoryrush/") + subdir + "initialized.dat");
            FILE *initfl = fopen(initpth.c_str(), "w");
            fprintf(initfl, "%d", 1);
            fclose(initfl);


            setlocale(LC_CTYPE, "UTF-8");

            Console::XtermInitTracking();

            Console::terminal_name = GetTerminalName(Console::ppid);

            tcgetattr(STDIN_FILENO,&old_termios);
            termios term_ios = old_termios;
            term_ios.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &term_ios);

            
            
            initialised = true;

            atexit(Fin);
            at_quick_exit(Fin);

            signal(SIGHUP, quick_exit);
            signal(SIGINT, quick_exit);
            signal(SIGQUIT, quick_exit);
            signal(SIGILL, quick_exit);
            signal(SIGTRAP, quick_exit);
            signal(SIGABRT, quick_exit);
            signal(SIGIOT, quick_exit);
            signal(SIGFPE, quick_exit);
            signal(SIGKILL, quick_exit);
            signal(SIGUSR1, quick_exit);
            signal(SIGSEGV, quick_exit);
            signal(SIGUSR2, quick_exit);
            signal(SIGPIPE, quick_exit);
            signal(SIGTERM, quick_exit);
        #ifdef SIGSTKFLT
            signal(SIGSTKFLT, quick_exit);
        #endif
            signal(SIGCHLD, quick_exit);
            signal(SIGCONT, quick_exit);
            signal(SIGSTOP, quick_exit);
            signal(SIGTSTP, quick_exit);
            signal(SIGTTIN, quick_exit);
            signal(SIGTTOU, quick_exit);

        }
    }

    void Console::Fin(void) {
        if (initialised) {

            for (size_t i = 0; i < Console::popup_pids.size(); i++)
                kill(Console::popup_pids[i], SIGTERM);

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);
            
            XtermFinishTracking();

            initialised = false;
        }
    }

    void Console::HandleKeyboard(void) {
        // toggles
        Console::keys_toggled.CapsLock = capslocktoggled();

        // press - release
        key_hit = -1;
        key_released = -1;
        for (int i = 0; i < KEYBOARD_MAX; i++) {
            bool keyState = getkeystate(i);
            if (!key_states[i] && keyState) key_hit = i;
            if (key_states[i] && !keyState) key_released = i;
            
            key_states[i] = keyState;
        }
    }

    void Console::HandleMouseAndFocus(void) {
        Console::XtermMouseAndFocus();
    }

    static string getProcessName(pid_t pid) {
        char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
        int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
        if (ret <= 0) {
            fprintf(stderr, "proc_pidpath() failed: %s\n", strerror(errno));
            exit(0x90);
        }
        return string(pathbuf);
    }
    
    static pid_t getParentPid(pid_t pid) {
        proc_bsdshortinfo pinfo = proc_bsdshortinfo();
        int ret = proc_pidinfo(pid, PROC_PIDT_SHORTBSDINFO, 0, &pinfo, sizeof(pinfo));
        if (ret <= 0) {
            fprintf(stderr, "proc_pidpath() failed: %s, pid: %d\n", strerror(errno), pid);
            exit(0x90);
        }
        return pinfo.pbsi_ppid;
    }
    
    static bool isApp(pid_t pid) {
        string name = getProcessName(pid);
        return name.find(".app") != string::npos;
    }

    static string GetTerminalName(pid_t pid) {
        while (!isApp(pid))
            if (pid) pid = getParentPid(pid);
            else return string();
        string name = getProcessName(pid);
    search:
        while (name[name.size()-1] != 'p' || name[name.size()-2] != 'p' || name[name.size()-3] != 'a' || name[name.size()-4] != '.')
            name.pop_back();
        if (name.substr(0, name.size()-5).find(".app") != string::npos) {
            name = name.substr(0, name.size()-5);
            goto search;
        }
        return name;
    }

    utfstr Console::GetTerminalExecutableName(void) {
        return Console::terminal_name;;
    }


    Key::Enum Console::KeyPressed(void) {
        if (Console::key_hit < 0) return Key::Enum::NONE;
        return static_cast<Key::Enum>(Console::key_hit);
    }

    Key::Enum Console::KeyReleased(void) {
        if (Console::key_released < 0) return Key::Enum::NONE;
        return static_cast<Key::Enum>(Console::key_released);
    }

    bool Console::IsKeyDown(Key::Enum key) {
        return Console::key_states[static_cast<unsigned short>(key)];
    }

    pid_t Console::ppid = 0;
#elif __CYGWIN__
void Console::Init(void) {
    if (!initialised) {
        
        int sub_process = 0;
        auto winargv = (const wchar_t**)CommandLineToArgvW(GetCommandLine(), &Console::argc);
        Console::argv = (const wchar_t**)malloc(sizeof(wchar_t*) * Console::argc);
        int j = 0;
        for (int i = 0; i < Console::argc; i++) {
            auto&& arg = winargv[i+j];
            if (wcslen(arg) > 1 && arg[0] == L'\033') {
                wstring sdir; const wchar_t* narg;
                switch (arg[1]) {
                    case L'&':
                        // launched as popup
                        Console::sub_proc = true;
                        if (wcslen(arg) < 3) ThrowMsg("Invalid argument 1");
                        sub_process = 0;
                        narg = arg + 2;
                        while (narg[0] != L'~') {
                            if (narg[0] < L'0' || narg[0] > L'9') ThrowMsg("Invalid argument 2");
                            sub_process *= 10;
                            sub_process += narg[0] - L'0';
                            ++narg;
                        }
                        for (size_t i = 1; narg[i] != L'~'; i++){
                            if (narg[i] == L'\0') ThrowMsg("Invalid argument 3");
                            sdir.push_back(narg[i]);
                        }
                        Console::subdir = new const wchar_t[wcslen(sdir.c_str())+1]{0};
                        wcscpy((wchar_t*)Console::subdir, sdir.c_str());
                        ++j; --i; --Console::argc;
                        break;
                    case L'\\':
                        ThrowMsg("IDK Argument");
                        break;
                    default:
                        Console::argv[i] = arg;
                        break;
                }
            } else Console::argv[i] = arg;
        }

        auto _temp = realloc(Console::argv, sizeof(wchar_t*) * Console::argc);
        if (_temp) Console::argv = (const wchar_t**)_temp;
        else exit(0x73);

        wchar_t* tmpappdata = _wgetenv(L"APPDATA");
        if (!tmpappdata) ThrowMsg("\"APPDATA\" env variable not set");
        Console::user_data = tmpappdata;
        user_data.append(L"\\.factoryrush\\");

        wchar_t* tmptmpdata = _wgetenv(L"TEMP");
        if (!tmptmpdata) ThrowMsg("\"TEMP\" env variable not set");
        Console::tmp_data = tmptmpdata;
        tmp_data.append(L"\\.factoryrush\\");

        wchar_t* tmpdevdata = _wgetenv(L"ProgramData");
        if (!tmpdevdata) ThrowMsg("\"ProgramData\" env variable not set");
        Console::dev_data = tmpdevdata;
        dev_data.append(L"\\Factoryrush\\");

        LPSECURITY_ATTRIBUTES sec_atrs = new SECURITY_ATTRIBUTES();
        sec_atrs->nLength = sizeof(sec_atrs);
        sec_atrs->lpSecurityDescriptor = nullptr;
        sec_atrs->bInheritHandle = false;

        Console::pid = _getpid();

        if (Console::sub_proc) {
            FILE* fl = _wfopen((tmp_data+subdir+L"pid.dat").c_str(), L"w");
            fwrite(&Console::pid, sizeof(Console::pid), 1, fl);
            fclose(fl);
            goto subdirset;
        }
        
        if (System::MakeDirectory(tmp_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + tmp_data + L"\"");

        if (System::MakeDirectory(user_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + user_data + L"\"");

        if (System::MakeDirectory(dev_data.c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + dev_data + L"\"");

        if (System::MakeDirectory((dev_data+L"logs").c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (dev_data+L"logs") + L"\"");

        Console::subdir = new const wchar_t[to_wstring(Console::pid).size() + 2]{0};
        wcscpy((wchar_t*)Console::subdir, to_wstring(Console::pid).c_str());
        ((wchar_t*)Console::subdir)[to_wstring(Console::pid).size()] = L'\\';
        
    subdirset:

        if (System::MakeDirectory((tmp_data+subdir).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (tmp_data+subdir) + L"\"");

        if (System::MakeDirectory((tmp_data+subdir+L"proc").c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg(wstring(L"Couldn't create directory: \"") + (tmp_data+subdir+L"proc") + L"\"");
        
        delete sec_atrs;

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        wstringstream ss; ss << dev_data << L"logs\\";
        if (Console::sub_proc) ss << sub_process << L'\\';
        ss << std::put_time(&tm, L"%Y-%m-%d_%H\'%M\'%S");
        unsigned int filenum = 0;
        auto sstr = ss.str();
    logfile:
        filesystem::path out_file = filesystem::path(sstr + L".log");
        HANDLE logfl = CreateFile(out_file.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (logfl == (void*)-1) {
            auto err = GetLastError();
            if (err == ERROR_ALREADY_EXISTS) {
                sstr = ss.str() + L"-" + to_wstring(++filenum);
                goto logfile;
            }
            Console::out << L"Couldn't create log file: " << out_file.native() << L", with error: " << err << '\n';
            exit(0xC2);
        }
        CloseHandle(logfl);
        Console::real_out.open(out_file,ios::out);
        if (!Console::real_out.is_open()) {
            wcerr << L"Couldn't open log file: " << out_file.native() << L", with error: " << GetLastError() << '\n'; 
            exit(0xC1);
        }

        Console::screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(Console::screen);
        
        Console::fd = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(Console::fd, &Console::old_console);
        SetConsoleMode(Console::fd, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

        Console::input_thread_arg = (void*)new __getinput_arg{Console::input_buf, SLEEP_THREAD_INPUT};
        Console::input_thread = CreateThread(NULL, 0, InputThread, input_thread_arg, 0, NULL);
        *Console::super_thread_run = true;
        Console::super_thread_arg = (void*)new __superthread_arg{Console::super_thread_run, Console::thread_handles};
        Console::super_thread = CreateThread(NULL, 0, Console::SuperThread, Console::super_thread_arg, 0, NULL);

        auto term_prog = _wgetenv(L"TERM_PROGRAM");
        mintty = term_prog && wstring(term_prog) == L"mintty";
        wt = _wgetenv(L"WT_SESSION") != NULL;
        Console::window = GetHwnd();
        Console::device = GetDC(NULL);
        //Console::xyoffset = Console::GetXYCharOffset();

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(Console::screen, &csbi);
        auto val = AtrValToColors(csbi.wAttributes);
        Console::default_fcol = val.first;
        Console::default_bcol = val.second;

        GetConsoleCursorInfo(Console::screen, &Console::old_curinf);
        Console::cursor_size = old_curinf.dwSize;
        Console::cursor_visible = old_curinf.bVisible;
        
        Console::mouse_status.x = 0;
        Console::mouse_status.y = 0;
        
        initialised = true;

        atexit(Console::Fin);
        at_quick_exit(Console::Fin);

        signal(SIGINT, quick_exit);
        signal(SIGILL, quick_exit);
        signal(SIGABRT, quick_exit);
        signal(SIGFPE, quick_exit);
        signal(SIGSEGV, quick_exit);
        signal(SIGTERM, quick_exit);
        signal(SIGBREAK, quick_exit);

        if (sub_process) {
            Console::ret = sub(sub_process);
            Console::Fin();
            exit(Console::ret);
        } else if (sub_proc) {
            Console::ret = Main();
            Console::Fin();
            exit(Console::ret);
        }
    }
}

#else

// NO APPLE NO LINUX
#error "Not implemented... yet"
    void Console::Init(void) {
        if (!initialised) {

            fwrite("\033[?1049h",sizeof(char), 8, stderr);

            tcgetattr(STDIN_FILENO,&old_termios);
            termios term_ios = old_termios;
            term_ios.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &term_ios);
            
            initialised = true;

            atexit(Fin);
            at_quick_exit(Fin);

            signal(SIGHUP, quick_exit);
            signal(SIGINT, quick_exit);
            signal(SIGQUIT, quick_exit);
            signal(SIGILL, quick_exit);
            signal(SIGTRAP, quick_exit);
            signal(SIGABRT, quick_exit);
            signal(SIGIOT, quick_exit);
            signal(SIGFPE, quick_exit);
            signal(SIGKILL, quick_exit);
            signal(SIGUSR1, quick_exit);
            signal(SIGSEGV, quick_exit);
            signal(SIGUSR2, quick_exit);
            signal(SIGPIPE, quick_exit);
            signal(SIGTERM, quick_exit);
        #ifdef SIGSTKFLT
            signal(SIGSTKFLT, quick_exit);
        #endif
            signal(SIGCHLD, quick_exit);
            signal(SIGCONT, quick_exit);
            signal(SIGSTOP, quick_exit);
            signal(SIGTSTP, quick_exit);
            signal(SIGTTIN, quick_exit);
            signal(SIGTTOU, quick_exit);

        }
    }

    void Console::Fin(void) {
        if (initialised) {

            for (size_t i = 0; i < Console::popup_pids.size(); i++)
                kill(Console::popup_pids[i], SIGTERM);

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);
            
            fwrite("\033[?1049l",sizeof(char), 8, stderr);

            initialised = false;
        }
    }
#endif

// all non windows
    inline clang_constexpr string GenerateEscapeSequence(uint8_t i1, uint8_t i2) {
        string val = "\033[";
        if (i1 < 8) {
            val.append(to_string(30 + i1));
        } else if (i1 < 16) {
            val.append(to_string(82 + i1));
        } else {
            val.append("39");
        }
        val.push_back(';');
        if (i2 < 8) {
            val.append(to_string(40 + i2));
        } else if (i2 < 16) {
            val.append(to_string(92 + i2));
        } else {
            val.append("49");
        }
        val.push_back('m');
        return val;
    }


    int16_t Console::GetWindowWidth(void) {
        ioctl(STDERR_FILENO, TIOCGWINSZ, &Console::window_size);
        return Console::window_size.ws_col;
    }

    int16_t Console::GetWindowHeight(void) {
        ioctl(STDERR_FILENO, TIOCGWINSZ, &Console::window_size);
        return Console::window_size.ws_row;
    }

    void Console::FillScreen(const vector<vector<Console::Symbol> >& symbols) {
        if (!Console::stderr_lock.try_lock()) return; // it's a slow function so we can't put multiple in a queue
        string screen = "\033[?25l\033[H";
        bool moved = false;
        size_t width = GetWindowWidth(), height = GetWindowHeight();

        if ((int16_t)width != Console::old_scr_size.first || (int16_t)height != Console::old_scr_size.second) {
            Console::old_symbols.clear();
        }

        int icounter = 0;
        for (size_t i = 0; i < height; i++) {
            if (i < Console::old_symbols.size()) {
                int jcounter = 0;
                if (i >= symbols.size()) {
                    bool reset = false;
                    for (size_t j = 0; j < width; j++)
                        if (j < old_symbols[i].size() && (old_symbols[i][j].character[0] != ' ' ||  old_symbols[i][j].foreground != 16 || old_symbols[i][j].background != 16)) {
                            if (icounter) screen.append(string("\033[") + to_string(icounter) + "E");
                            if (jcounter) screen.append(string("\033[") + to_string(jcounter) + "C");
                            if (!reset) screen.append("\033[0m");
                            screen.push_back(' ');
                            icounter = jcounter = !(reset = true);
                        } else ++jcounter;
                } else {
                    for (size_t j = 0; j < width; j++) {
                        if (j >= symbols[i].size()) {
                            bool reset = false; --j;
                            while (++j < width)
                                if (j < old_symbols[i].size() && (old_symbols[i][j].character[0] != ' ' ||  old_symbols[i][j].foreground != 16 || old_symbols[i][j].background != 16)) {
                                    if (icounter) screen.append(string("\033[") + to_string(icounter) + "E");
                                    if (jcounter) screen.append(string("\033[") + to_string(jcounter) + "C");
                                    if (!reset) screen.append("\033[0m");
                                    screen.push_back(' ');
                                    icounter = jcounter = !(reset = true);
                                } else ++jcounter;
                        }
                        else if ((j >= old_symbols[i].size() && (symbols[i][j].character[0] != ' ' ||  symbols[i][j].foreground != 16 || symbols[i][j].background != 16)) || (old_symbols[i][j].character != symbols[i][j].character || old_symbols[i][j].foreground != symbols[i][j].foreground || old_symbols[i][j].background != symbols[i][j].background)) {
                            if (icounter) screen.append(string("\033[") + to_string(icounter) + "E");
                            if (jcounter) screen.append(string("\033[") + to_string(jcounter) + "C");
                            screen.append( GenerateEscapeSequence(symbols[i][j].foreground, symbols[i][j].background) );
                            screen.append(symbols[i][j].character);
                            icounter = jcounter = 0;
                        } else ++jcounter;
                    }
                }
                if (jcounter) ++icounter;
                else screen.append("\033[E");
                continue;
            }
            if (icounter) screen.append(string("\033[") + to_string(icounter) + "E");
            else if (i) screen.append("\n");
            icounter = 0;
            if (i >= symbols.size()) {
                if (width) screen.append("\033[0m");
                for (size_t j = 0; j < width; j++)
                    screen.push_back(' ');
            }
            else
                for (size_t j = 0; j < width; j++) {
                    if (j >= symbols[i].size()) {
                        screen.append("\033[0m ");
                        while (++j < width) screen.push_back(' ');
                        break;
                    }
                    screen.append( GenerateEscapeSequence(symbols[i][j].foreground, symbols[i][j].background) );
                    screen.append(symbols[i][j].character);
                }
        }
        if (screen.size() < 10) { 
            //fprintf(stdout, "Terminal chars: ~%d, written none\n", width * height * 10, 0);
            return;
        }
        screen.append("\033[0m");
        fwrite(screen.c_str(), sizeof(char), screen.size(), stderr);
        old_scr_size = {width,height};
        old_symbols = symbols;
        Console::stderr_lock.unlock();
        Console::EscSeqMoveCursor();
        Console::EscSeqSetCursor();
        //fprintf(stdout, "Terminal chars: ~%d, written chars: %d", width * height * 10, screen.size());
        //fwrite(screen.c_str(), sizeof(char), screen.size(), stdout);
        //fputc('\n', stdout);
        //fflush(stdout);
    }

    void SysSleep(int microseconds){
        int ret;
        if (!microseconds % 1000000) ret = 1000000 * sleep(microseconds / 1000000);
        else ret = usleep(microseconds);
        if (ret) SysSleep(ret);
    }

    struct termios Console::old_termios = termios();
    winsize Console::window_size = winsize();
    const char* Console::subdir = nullptr;
#endif

basic_istringstream<wchar_t> Console::in = basic_istringstream<wchar_t>(std::ios_base::ate|std::ios_base::in);

#ifdef _WIN32
    basic_ostringstream<wchar_t> Console::out = basic_ostringstream<wchar_t>();
#else
    basic_ostringstream<wchar_t> Console::out = basic_ostringstream<wchar_t>();
#endif

int Console::argc = 0;
utfcstr* Console::argv = (utfcstr*)malloc(sizeof(utfcstr));

bool Console::emulator = false;
bool Console::initialised = false;
mutex Console::stderr_lock = {};
bitset<KEYBOARD_MAX> Console::key_states = bitset<KEYBOARD_MAX>(0);
struct ToggledKeys Console::keys_toggled = ToggledKeys();
bitset<16> Console::mouse_buttons_down = bitset<16>(0);
struct Console::MouseStatus Console::mouse_status = Console::MouseStatus();
std::chrono::time_point<std::chrono::high_resolution_clock> Console::last_click_time = std::chrono::high_resolution_clock::now();

int Console::key_hit = int();
int Console::key_released = int();
uint8_t Console::this_mouse_button = uint8_t(-1);
uint8_t Console::this_mouse_combo = uint8_t(0);
bool Console::this_mouse_down = bool(false);
uint8_t Console::last_mouse_button = uint8_t(-1);
uint8_t Console::last_mouse_combo = uint8_t(0);
bool Console::focused = bool(true);
#ifdef _WIN32
    atomic<bool> Console::tabactive = bool(true);
#endif
unsigned short Console::double_click_max = (unsigned short)(500);
pid_t Console::pid = pid_t(-1); 
int Console::ret = int(-1);
bool Console::sub_proc = bool(false);
char_t Console::buf[127] = N("\0");
int8_t Console::buf_it = -1;
uint16_t Console::next_pid = uint16_t(10000);
array<bool,UINT16_MAX> Console::used_pids = array<bool,UINT16_MAX>{0};
pair<int16_t,int16_t> Console::cursorpos = pair<uint16_t,uint16_t>(0,0);
uint8_t Console::cursor_size = uint8_t(100);
utfstr Console::window_title = utfstr(N(" "));
bool Console::cursor_visible = bool(true);
bool Console::cursor_blink_opposite = bool(false);
Console::config_t Console::config = Console::config_t();
std::atomic<bool> Console::refresh_screen = bool(false);
vector<vector<Console::Symbol>> Console::old_symbols = vector<vector<Console::Symbol>>();
pair<int16_t,int16_t> Console::old_scr_size = pair<int16_t,int16_t>(0,0);
utfstr Console::tmp_data = utfstr();
utfstr Console::user_data = utfstr();
utfstr Console::dev_data = utfstr();
vector<pid_t> Console::popup_pids = vector<pid_t>();
wstring Console::terminal_name = wstring();
int Console::max_popup_startup_wait = int(15000);

struct ToggledKeys Console::KeysToggled(void) {
    return Console::keys_toggled;
}

bool Console::IsFocused(void) {
    return Console::focused
#ifdef _WIN32
    && (!Console::parent_window || (GetForegroundWindow() == Console::parent_window))
    && Console::tabactive
#endif
    ;
}

struct Console::MouseStatus Console::GetMouseStatus(void) {
    return Console::mouse_status;
}

pair<uint8_t, uint8_t> Console::MouseButtonClicked(void) {
    return Console::this_mouse_down ? pair<uint8_t, uint8_t>(Console::this_mouse_button, Console::this_mouse_combo) : pair<uint8_t, uint8_t>(-1,0);
}

uint8_t Console::MouseButtonReleased(void) {
    return !Console::this_mouse_down ? Console::this_mouse_button : -1;
}

bool Console::IsMouseButtonDown(uint8_t button) {
    return Console::mouse_buttons_down[button];
}

void Console::Sleep(double seconds, bool sleep_input_thread) {
#ifdef _WIN32
    HANDLE Hinput_thread = Console::thread_handles->front(); // input thread was added first
    if (sleep_input_thread)
        SuspendThread(Hinput_thread);
#endif

    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;

    while (seconds > estimate) {
        auto start = high_resolution_clock::now();
        SysSleep(1000);
        auto end = high_resolution_clock::now();

        double observed = (end - start).count() / 1e9;
        seconds -= observed;

        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2   += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    auto start = high_resolution_clock::now();
    while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);

#ifdef _WIN32
    if (sleep_input_thread)
        ResumeThread(Hinput_thread);
#endif
}

void Console::SetDoubleClickMaxWait(unsigned short milliseconds) {
    Console::double_click_max = milliseconds;
}

unsigned short Console::GetDoubleClickMaxWait(void) {
    return Console::double_click_max;
}

int32_t Console::GetArgC(void) {
    return Console::argc;
}

utfcstr* Console::GetArgV(void) {
    return Console::argv;
}

Console::MouseStatus::MouseStatus(void) {
    this->primary = false;
    this->middle = false;
    this->secondary = false;
    this->scroll = pair<bool,bool>(false,false);
    this->x = 0;
    this->y = 0;
}

Console::Symbol::Symbol(utfchar character, uint8_t foreground, uint8_t background) {
    this->character = character;
    this->foreground = foreground;
    this->background = background;
}

Console::Symbol::Symbol(void) {
    this->character = N(' ');
    this->foreground = 7;
    this->background = 0;
}

Console::Symbol::Symbol(const Symbol& sym) {
    this->character = sym.character;
    this->foreground = sym.foreground;
    this->background = sym.background;
}

Console::Symbol::~Symbol(void) {}

void Console::Symbol::ReverseColors(void) {
    cppimp::Console_Symbol_ReverseColors(this);
}

Console::Symbol & Console::Symbol::operator=(const Console::Symbol & src) {
    if (this == &src) { return *this; }
    this->character = src.character;
    this->background = src.background;
    this->foreground = src.foreground;
    return *this;
}

void Console::ClearScreenBuffer(void) {
    Console::out << L"ClearScreenBuffer" << L'\n';
    Console::out_flush();
    Console::refresh_screen = true;
}
void Console::Exit(int code) {
    Console::ret = code;
    Console::Fin();
    exit(code);
}

void Console::QuickExit(int code) {
    Console::ret = code;
    Console::Fin();
    quick_exit(code);
}

optional<pair<int,utfstr>> cpp::Console::PopupWindow(int type, int argc, const char_t* argv[], const char_t* title) {
    auto term = Console::GetTerminalExecutableName();

#ifdef _WIN32
    HANDLE Hinput_thread = Console::thread_handles->front(); // input thread was added first
    SuspendThread(Hinput_thread);
#endif

    int pc = 0;
newpidgen:
    if (++pc > UINT16_MAX) exit(0xF1); // no pid left
    if (Console::next_pid == UINT16_MAX) Console::next_pid = 0;
    auto& npid = ++Console::next_pid;
    if (Console::used_pids[npid]) goto newpidgen;
    Console::used_pids[npid] = true;

    utfcstr* args = (utfcstr*)System::AllocateMemory(sizeof(void*) * (argc+3
#ifdef _WIN32
    + conemu*7 // conemu need 7 more args
    - cmder // cmder uses one less arg than conemu
#else
    + tabby // tabby need 1 more arg
#endif
    ));

#if defined(_WIN32) || defined(__CYGWIN__)
    #define sep L'\\'
    #define topen _wfopen
    #define fgetnc fgetwc
#else
    #define sep '/'
    #define topen fopen
    #define fgetnc fgetc
    #define ERROR_PATH_NOT_FOUND ENOENT

    char* appdata = getenv("HOME");
    const char* tmppath = "/tmp";
#endif
    utfstr procdir = utfstr(N("proc")) + sep + to_nstring(npid) + sep;
    if (System::MakeDirectory((Console::tmp_data + subdir + procdir).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::tmp_data + sep + subdir + procdir + N("\"")).c_str());
    if (System::MakeDirectory((Console::dev_data + N("logs") + sep + to_nstring(type) + sep).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::dev_data + sep + N("logs") + sep + to_nstring(type) + sep + N("\"")).c_str());
    utfstr info = N("\033&");
    info.append(to_nstring(type)).push_back(N('~'));
    info.append(subdir).append(procdir).push_back(N('~'));
    utfstr root_pth = System::GetSelfPath();
#ifdef __linux__
    auto termchk = term;
#else
    // ".exe", ".app"
    auto termchk = term; termchk.pop_back(); termchk.pop_back(); termchk.pop_back(); termchk.pop_back();
#endif
    // Hyper doesn't have a cli option to run a program, so we have to use the default term
    if (termchk[termchk.size()-1] == N('r') && termchk[termchk.size()-2] == N('e') && termchk[termchk.size()-3] == N('p') && termchk[termchk.size()-4] == N('y') && termchk[termchk.size()-5] == N('H'))
        term.clear();

#ifdef _WIN32
    wstring term2;
    wstring newconstitle, newconsicon, newwinicon;
    int tabby2 = 0;

    if (mintty && !conemu && !good) {
        if (term.find(L"mintty") == std::wstring::npos) {
            Console::window = GetForegroundWindow();
        }
        term = Console::GetTerminalExecutableName();
        if (!(good = term.find(L"mintty") != std::wstring::npos))
            goto console;
    } else if (!term.size())
        goto console;
#elif __CYGWIN__
    if (!term.size()) {
        term.append(L"C:\\cygwin64\\bin\\mintty.exe"); // todo - if mintty doesn't exists the use default 
        if (false) goto console;
    }
#elif __APPLE__
    if (!term.size()) {
        term.append("/System/Applications/Utilities/Terminal.app"); // idk if this is the actual path
    }
#elif __linux__
    if (!term.size()) {
        term.append("/usr/bin/gnome-terminal"); // todo - find default terminal emulator
    }
#else
    if (!term.size()) {
        return nullopt;
    }
#endif
    args[0] = root_pth.c_str();
    args[1] = info.c_str();
    for (int i = 1; i <= argc; i++) args[i+1] = argv[i-1];
    args[argc+2] = nullptr;
#ifdef _WIN32 // for some reason, WindowsTerminal.exe doesn't work, but wt.exe does
    if (wt) {
        term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"wt.exe");
        if (System::RunProgramAsync(term2.c_str(), args)) goto contcons;
    }
    if (tabby) {
        /* need to close the window after the popup exits
         * so this creates a powershell script that run's the popup and kill the parent process
         * and creates a batch file that runs the powershell script
         * than runs the `term` to create a new window
         * and after launch runs the batch file with the `term`
        */
       {
            wstring runps = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".ps1";
            auto file = _wfopen(runps.c_str(), L"w");
            _setmode(_fileno(file),  _O_WTEXT); wint_t x = 0xfeff; fwrite(&x, sizeof(wint_t), 1, file);

            fwrite(L"del ($MyInvocation.MyCommand).Path\ndel \"$(Split-Path -Parent -Path $($MyInvocation.MyCommand).Path)\\", sizeof(wchar_t), 100, file);
            fwrite(to_wstring(npid).c_str(), sizeof(wchar_t), to_wstring(npid).size(), file);
            fwrite(L".cmd\"\n& ", sizeof(wchar_t), 8, file);
            for (int i = 0; args[i]; i++)
                fwprintf(file, L"\"%ls\" ", args[i]);
            fwprintf(file, L"\n%ls\\bin\\killwindow.exe", System::GetRootDir().c_str());
            fclose(file);

            wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
            file = _wfopen(runpth.c_str(), L"w");
            fwrite("@echo off\npowershell -NoProfile -ExecutionPolicy Bypass -Command \"%~dp0", sizeof(char), 71, file);
            fwrite(to_string(npid).c_str(), sizeof(char), to_string(npid).size(), file);
            fwrite(".ps1\"\n", sizeof(char), 5, file);
            fclose(file);

            if (!System::RunProgramAsync(term.c_str(), (const wchar_t*)nullptr)) /* Run to launch new window */ {
                ResumeThread(Hinput_thread);
                return nullopt;
            }
            DWORD processId;
            GetWindowThreadProcessId(Console::window, &processId);
            unordered_map<DWORD, bool> processes;
            {
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    processes[prc] = true;
                }
            }
            while (1) {
                SysSleep(1000);
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    if (processes[prc]) continue;
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
                    if (processHandle) {
                        wchar_t exePath[MAX_PATH];
                        if (GetModuleFileNameEx(processHandle, NULL, exePath, MAX_PATH)) {
                            Console::out << L"New child process: " << exePath; out_endl();
                            tabby = false;
                            ModifyExecutableName(exePath);
                            if (tabby) {
                                Console::out << L"Tabby process\n"; out_endl();
                                auto thiswin = GetForegroundWindow();
                                while (thiswin == GetForegroundWindow()) SysSleep(1000);
                                Console::Sleep(0.9);
                                goto runtabby;
                            } else tabby = true;
                        }
                    }
                    processes[prc] = true;
                }
            }
        }
    runtabby:
        wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
        const wchar_t** realargs = (const wchar_t**)System::AllocateMemory(sizeof(void*) * (5));
        realargs[0] = L"run";
        realargs[1] = runpth.c_str();
        realargs[2] = L";";
        realargs[3] = L"exit";
        realargs[4] = nullptr;
        Console::out << L"Running: " << term << L" " << runpth; out_endl();
        if (!System::RunProgramAsync(term.c_str(), realargs)) {
            ResumeThread(Hinput_thread);
            return nullopt;
        }
        Console::out << L"Started: " << term << L" " << runpth; out_endl();
        System::FreeMemory(realargs);
        goto contcons;
    }
    else if (conemu) {
        for (int i = argc+2+7-cmder; i >= 7-cmder; i--) args[i] = args[i-(7-cmder)]; // cmder desn't need args[6]
        args[0] = L"-Title";
        args[1] = title ? title : Console::window_title.c_str();
        args[2] = L"-cmd"; // new versions also support "-run", but old ones don't
        // (when new vesions stop supporting '-cmd' I'll have to get the version somehow)
        newconstitle = L"-new_console:t:"; newconstitle.append(args[1]);
        args[3] = newconstitle.c_str();
        newconsicon = L"-new_console:C:"; newconsicon.append(System::GetRootDir()).append(L"\\assets\\images\\icon.ico");
        args[4] = newconsicon.c_str();
        if (cmder) {
            term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"vendor\\git-for-windows\\usr\\bin\\mintty.exe");
            args[5] = term2.c_str();
            wstring cmdargs;
            for (int i = 0; i < argc + 2 + 7 - 1; i++)
                cmdargs.append(L"\"").append(args[i]).append(L"\" ");
            cmdargs.pop_back();
            const wchar_t* realargs[3] = { L"/x", cmdargs.c_str(), nullptr };
            if (!System::RunProgramAsync(term.c_str(), realargs)) {
                ResumeThread(Hinput_thread);
                return nullopt;
            }
            goto contcons;
        } else {
            args[6] = args[4];
            args[5] = args[3];
            args[4] = args[2];
            args[2] = L"-icon";
            newwinicon = (System::GetRootDir() + L"\\assets\\images\\icon.ico");
            args[3] = newwinicon.c_str();
        }
    }
#else
    if (tabby) {
        for (int i = argc+2; i > 0; i--) args[i] = args[i-1];
        args[0] = L"run";
    }
#endif
#ifndef __APPLE__
    if (!System::RunProgramAsync(term.c_str(), args)) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
#else
    string runpth = string("/tmp/.factoryrush/") + subdir + "proc/" + to_nstring(npid) + ".command";
    auto file = fopen(runpth.c_str(), "w");
    fwrite("#!/bin/sh\nrm -f $0\n", sizeof(char), 19, file);
    for (int i = 0; args[i]; i++)
        { fwrite("\"",sizeof(char),1,file); fwrite(args[i], sizeof(char), strlen(args[i]), file); fwrite("\" ", sizeof(char), 2, file); }
    fwrite("\n", sizeof(char), 1, file);
    string killtrmpth = root_pth; while (killtrmpth.back() != '/') killtrmpth.pop_back();
    killtrmpth += "../share/factoryrush/bin/killterm";
    fwrite(killtrmpth.c_str(), sizeof(char), killtrmpth.size(), file);
    fwrite("\n", sizeof(char), 1, file);
    fchmod(fileno(file), 0755);
    fclose(file);
    if (openFileInApp(runpth.c_str(), term.c_str()) != 0) return nullopt;
#endif
#ifdef _WIN32
    goto contcons;
console:
    for (int i = 0; i < argc; i++) args[i] = argv[i];
    args[argc] = info.c_str();
    args[argc+1] = nullptr;
    if (!System::RunProgramAsyncC(root_pth.c_str(), args)) {
        ResumeThread(Hinput_thread);
        return nullopt;
    }
contcons:
    if (!tabby2)
#endif
    System::FreeMemory(args);

    // maybe don't wait or do it on a thread [?]
    Console::out << L"Waiting for popup to start until: " << max_popup_startup_wait; out_endl();
    auto start = chrono::high_resolution_clock::now();
    while ((!System::IsFile((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str())) && (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < max_popup_startup_wait)) {
    #ifdef _WIN32
        if (tabby && tabby2 < 5 && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() + 1000 > (max_popup_startup_wait/3)*(max(min(2,tabby2),1)) && System::IsFile((Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd").c_str())) {
            Console::out << L"Try again " << ++tabby2; out_endl();
            goto runtabby;
        }
    #endif
        SysSleep(1000);
        Console::out << L"Waiting for popup to start: " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count(); out_endl();
    }
    if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() >= max_popup_startup_wait) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    Console::out << L"Popup launched successfully" << L'\n';
    Console::HandleOutput();
    FILE* fl = topen((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str(), N("r"));
    if (!fl) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    pid_t spid = 0;
    fread(&spid, sizeof(pid_t), 1, fl);
    fclose(fl);
    Console::popup_pids.push_back(spid);

#ifdef _WIN32
    fl = topen((Console::tmp_data + subdir + procdir + N("window.dat")).c_str(), N("r"));
    if (!fl) {
        ResumeThread(Hinput_thread);
        return nullopt;
    }
    HWND swindow = 0;
    fread(&swindow, sizeof(HWND), 1, fl);
    fclose(fl);
    //cerr << Console::parent_window << ' ' << Console::window; out_endl();
    HWND twindow = Console::parent_window;
    bool correct_window = twindow, notme = true;
    DWORD this_thread = GetCurrentThreadId(), owner_thread = 0;
    if (twindow) {
        owner_thread = GetWindowThreadProcessId(twindow, nullptr);
        if ((notme = (this_thread != owner_thread)))
            AttachThreadInput(owner_thread, this_thread, true);
    }
#endif

    bool oldfocus = true;
    int setwindow = 0;
    auto olddisplay = Console::old_symbols;
    while (!System::IsFile((Console::tmp_data + subdir + procdir + N("exit.dat")).c_str())) {
        if (SLEEP_POPUP_CHECK) SysSleep(1000);
        Console::FillScreen(olddisplay);
        Console::HandleMouseAndFocus();
        if (IsFocused() && !oldfocus) Console::Beep();
    #ifdef _WIN32
        if (Console::focused) {
            DWORD tmpthread;
            if (!twindow) {
                Console::out << L'~';
                twindow = GetForegroundWindow();
                if (!twindow) continue;
                if (twindow == Console::window || twindow == Console::parent_window) {
                    correct_window = true;
                    cout << "Correct window"; out_endl();
                } else cout << "Wrong window"; out_endl();
                owner_thread = GetWindowThreadProcessId(twindow, nullptr);
                if ((notme = (this_thread != owner_thread)))
                    AttachThreadInput(owner_thread, this_thread, true);
            } else if (!correct_window) {
                Console::out << L'-';
                auto tmpwindow = GetForegroundWindow();
                if (!tmpwindow) continue;
                twindow = tmpwindow;
                if (tmpwindow == Console::window || tmpwindow == Console::parent_window) {
                    correct_window = true;
                    cout << "Correct window"; out_endl();
                    if (notme) AttachThreadInput(owner_thread, this_thread, false);
                    owner_thread = GetWindowThreadProcessId(twindow, nullptr);
                    if ((notme = (this_thread != owner_thread)))
                        AttachThreadInput(owner_thread, this_thread, true);
                }
            } else if (owner_thread != (tmpthread = GetWindowThreadProcessId(twindow, nullptr))) {
                if (notme) AttachThreadInput(owner_thread, this_thread, false);
                owner_thread = tmpthread;
                if ((notme = (this_thread != owner_thread)))
                    AttachThreadInput(owner_thread, this_thread, true);
            }
            if (!Console::tabactive) continue;
            if (cmder) {
                SysSleep(10000); // sleep 10ms to let the tab switch
                Console::HandleMouseAndFocus();
                if (!Console::tabactive || !Console::focused) continue;
            }
            int style = GetWindowLong(swindow, GWL_STYLE);
            if ((style & WS_MINIMIZE) == WS_MINIMIZE)
                ShowWindow(swindow, SW_RESTORE);
            BringWindowToTop(swindow);
            SetForegroundWindow(swindow);
            ShowWindow(swindow, SW_SHOW);
        }
    #endif
        if (Console::this_mouse_down && Console::this_mouse_button != MOUSE_SCROLL_UP && Console::this_mouse_button != MOUSE_SCROLL_DOWN)
            Console::Beep();
        oldfocus = IsFocused();
    }

#ifdef _WIN32
    if (twindow && notme)
        AttachThreadInput(owner_thread, this_thread, false);
#endif

    for (size_t i = 0; i < popup_pids.size(); i++)
        if (popup_pids[i] == spid) { popup_pids.erase(popup_pids.begin() + i); break; }

    fl = topen((Console::tmp_data + subdir + procdir + N("exit.dat")).c_str(), N("r"));
    if (!fl) ThrowMsg(utfstr(N("Couldn't open file: ")) + Console::tmp_data + subdir + procdir + N("exit.dat"));
    int pret = 0;
    fread(&pret, sizeof(int), 1, fl);
    fclose(fl);

    utfstr result;
    fl = topen((Console::tmp_data + subdir + procdir + N("result.dat")).c_str(), N("r"));
    if (!fl) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    unsigned long long len = 0;
    fread(&len, sizeof(unsigned long long), 1, fl);
    if (len) {
        char_t* buf = new char_t[len+1];
        fread(buf, sizeof(char_t), len, fl);
        buf[len] = 0;
        result = buf;
        delete[] buf;
    }

    System::RemoveFile((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str());
    System::RemoveFile((Console::tmp_data + subdir + procdir + N("exit.dat")).c_str());
    System::RemoveFile((Console::tmp_data + subdir + procdir + N("result.dat")).c_str());

    System::DeleteDirectory((Console::tmp_data + subdir + procdir).c_str());
    
    if (pret)
        Console::out << L"Popup exited with code: 0x" << std::hex << pret << L" and result: \"" << NativeToWString(result) << L"\"\n";


#ifdef _WIN32
    ResumeThread(Hinput_thread);
#endif
    return { {pret,result} };
}

std::optional<stsb::promise<std::optional<std::pair<int, uniconv::utfstr>>>> cpp::Console::PopupWindowAsync(int type, int argc, const char_t *argv[], const char_t *title) {
    auto term = Console::GetTerminalExecutableName();

#ifdef _WIN32
    HANDLE Hinput_thread = Console::thread_handles->front(); // input thread was added first
    SuspendThread(Hinput_thread);
#endif

    int pc = 0;
newpidgen:
    if (++pc > UINT16_MAX) exit(0xF1); // no pid left
    if (Console::next_pid == UINT16_MAX) Console::next_pid = 0;
    auto& npid = ++Console::next_pid;
    if (Console::used_pids[npid]) goto newpidgen;
    Console::used_pids[npid] = true;

    utfcstr* args = (utfcstr*)System::AllocateMemory(sizeof(void*) * (argc+3
#ifdef _WIN32
    + conemu*7 // conemu need 7 more args
    - cmder // cmder uses one less arg than conemu
#else
    + tabby // tabby need 1 more arg
#endif
    ));

#if defined(_WIN32) || defined(__CYGWIN__)
    #define sep L'\\'
    #define topen _wfopen
    #define fgetnc fgetwc
#else
    #define sep '/'
    #define topen fopen
    #define fgetnc fgetc
    #define ERROR_PATH_NOT_FOUND ENOENT

    char* appdata = getenv("HOME");
    const char* tmppath = "/tmp";
#endif
    utfstr procdir = utfstr(N("proc")) + sep + to_nstring(npid) + sep;
    if (System::MakeDirectory((Console::tmp_data + subdir + procdir).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::tmp_data + sep + subdir + procdir + N("\"")).c_str());
    if (System::MakeDirectory((Console::dev_data + N("logs") + sep + to_nstring(type) + sep).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::dev_data + sep + N("logs") + sep + to_nstring(type) + sep + N("\"")).c_str());
    utfstr info = N("\033&");
    info.append(to_nstring(type)).push_back(N('~'));
    info.append(subdir).append(procdir).push_back(N('~'));
    utfstr root_pth = System::GetSelfPath();
#ifdef __linux__
    auto termchk = term;
#else
    // ".exe", ".app"
    auto termchk = term; termchk.pop_back(); termchk.pop_back(); termchk.pop_back(); termchk.pop_back();
#endif
    // Hyper doesn't have a cli option to run a program, so we have to use the default term
    if (termchk[termchk.size()-1] == N('r') && termchk[termchk.size()-2] == N('e') && termchk[termchk.size()-3] == N('p') && termchk[termchk.size()-4] == N('y') && termchk[termchk.size()-5] == N('H'))
        term.clear();

#ifdef _WIN32
    wstring term2;
    wstring newconstitle, newconsicon, newwinicon;
    int tabby2 = 0;

    if (mintty && !conemu && !good) {
        if (term.find(L"mintty") == std::wstring::npos) {
            Console::window = GetForegroundWindow();
        }
        term = Console::GetTerminalExecutableName();
        if (!(good = term.find(L"mintty") != std::wstring::npos))
            goto console;
    } else if (!term.size())
        goto console;
#elif __CYGWIN__
    if (!term.size()) {
        term.append(L"C:\\cygwin64\\bin\\mintty.exe"); // todo - if mintty doesn't exists the use default 
        if (false) goto console;
    }
#elif __APPLE__
    if (!term.size()) {
        term.append("/System/Applications/Utilities/Terminal.app"); // idk if this is the actual path
    }
#elif __linux__
    if (!term.size()) {
        term.append("/usr/bin/gnome-terminal"); // todo - find default terminal emulator
    }
#else
    if (!term.size()) {
        return nullopt;
    }
#endif
    args[0] = root_pth.c_str();
    args[1] = info.c_str();
    for (int i = 1; i <= argc; i++) args[i+1] = argv[i-1];
    args[argc+2] = nullptr;
    #ifdef _WIN32 // for some reason, WindowsTerminal.exe doesn't work, but wt.exe does
    if (wt) {
        term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"wt.exe");
        if (System::RunProgramAsync(term2.c_str(), args)) goto contcons;
    }
    if (tabby) {
        /* need to close the window after the popup exits
         * so this creates a powershell script that run's the popup and kill the parent process
         * and creates a batch file that runs the powershell script
         * than runs the `term` to create a new window
         * and after launch runs the batch file with the `term`
        */
       {
            wstring runps = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".ps1";
            auto file = _wfopen(runps.c_str(), L"w");
            _setmode(_fileno(file),  _O_WTEXT); wint_t x = 0xfeff; fwrite(&x, sizeof(wint_t), 1, file);

            fwrite(L"del ($MyInvocation.MyCommand).Path\ndel \"$(Split-Path -Parent -Path $($MyInvocation.MyCommand).Path)\\", sizeof(wchar_t), 100, file);
            fwrite(to_wstring(npid).c_str(), sizeof(wchar_t), to_wstring(npid).size(), file);
            fwrite(L".cmd\"\n& ", sizeof(wchar_t), 8, file);
            for (int i = 0; args[i]; i++)
                fwprintf(file, L"\"%ls\" ", args[i]);
            fwprintf(file, L"\n%ls\\bin\\killwindow.exe", System::GetRootDir().c_str());
            fclose(file);

            wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
            file = _wfopen(runpth.c_str(), L"w");
            fwrite("@echo off\npowershell -NoProfile -ExecutionPolicy Bypass -Command \"%~dp0", sizeof(char), 71, file);
            fwrite(to_string(npid).c_str(), sizeof(char), to_string(npid).size(), file);
            fwrite(".ps1\"\n", sizeof(char), 5, file);
            fclose(file);

            if (!System::RunProgramAsync(term.c_str(), (const wchar_t*)nullptr)) {
                ResumeThread(Hinput_thread);
                return nullopt; // Run to launch new window
            }
            DWORD processId;
            GetWindowThreadProcessId(Console::window, &processId);
            unordered_map<DWORD, bool> processes;
            {
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    processes[prc] = true;
                }
            }
            while (1) {
                SysSleep(1000);
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    if (processes[prc]) continue;
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
                    if (processHandle) {
                        wchar_t exePath[MAX_PATH];
                        if (GetModuleFileNameEx(processHandle, NULL, exePath, MAX_PATH)) {
                            Console::out << L"New child process: " << exePath; out_endl();
                            tabby = false;
                            ModifyExecutableName(exePath);
                            if (tabby) {
                                Console::out << L"Tabby process\n"; out_endl();
                                auto thiswin = GetForegroundWindow();
                                while (thiswin == GetForegroundWindow()) SysSleep(1000);
                                Console::Sleep(0.9);
                                goto runtabby;
                            } else tabby = true;
                        }
                    }
                    processes[prc] = true;
                }
            }
        }
    runtabby:
        wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
        const wchar_t** realargs = (const wchar_t**)System::AllocateMemory(sizeof(void*) * (5));
        realargs[0] = L"run";
        realargs[1] = runpth.c_str();
        realargs[2] = L";";
        realargs[3] = L"exit";
        realargs[4] = nullptr;
        Console::out << L"Running: " << term << L" " << runpth; out_endl();
        if (!System::RunProgramAsync(term.c_str(), realargs)) {
            ResumeThread(Hinput_thread);
            return nullopt;
        }
        Console::out << L"Started: " << term << L" " << runpth; out_endl();
        System::FreeMemory(realargs);
        goto contcons;
    }
    else if (conemu) {
        for (int i = argc+2+7-cmder; i >= 7-cmder; i--) args[i] = args[i-(7-cmder)]; // cmder desn't need args[6]
        args[0] = L"-Title";
        args[1] = title ? title : Console::window_title.c_str();
        args[2] = L"-cmd"; // new versions also support "-run", but old ones don't
        // (when new vesions stop supporting '-cmd' I'll have to get the version somehow)
        newconstitle = L"-new_console:t:"; newconstitle.append(args[1]);
        args[3] = newconstitle.c_str();
        newconsicon = L"-new_console:C:"; newconsicon.append(System::GetRootDir()).append(L"\\assets\\images\\icon.ico");
        args[4] = newconsicon.c_str();
        if (cmder) {
            term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"vendor\\git-for-windows\\usr\\bin\\mintty.exe");
            args[5] = term2.c_str();
            wstring cmdargs;
            for (int i = 0; i < argc + 2 + 7 - 1; i++)
                cmdargs.append(L"\"").append(args[i]).append(L"\" ");
            cmdargs.pop_back();
            const wchar_t* realargs[3] = { L"/x", cmdargs.c_str(), nullptr };
            if (!System::RunProgramAsync(term.c_str(), realargs)) {
                ResumeThread(Hinput_thread);
                return nullopt;
            }
            goto contcons;
        } else {
            args[6] = args[4];
            args[5] = args[3];
            args[4] = args[2];
            args[2] = L"-icon";
            newwinicon = (System::GetRootDir() + L"\\assets\\images\\icon.ico");
            args[3] = newwinicon.c_str();
        }
    }
#else
    if (tabby) {
        for (int i = argc+2; i > 0; i--) args[i] = args[i-1];
        args[0] = L"run";
    }
#endif
#ifndef __APPLE__
    if (!System::RunProgramAsync(term.c_str(), args)) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
#else
    string runpth = string("/tmp/.factoryrush/") + subdir + "proc/" + to_nstring(npid) + ".command";
    auto file = fopen(runpth.c_str(), "w");
    fwrite("#!/bin/sh\nrm -f $0\n", sizeof(char), 19, file);
    for (int i = 0; args[i]; i++)
        { fwrite("\"",sizeof(char),1,file); fwrite(args[i], sizeof(char), strlen(args[i]), file); fwrite("\" ", sizeof(char), 2, file); }
    fwrite("\n", sizeof(char), 1, file);
    string killtrmpth = root_pth; while (killtrmpth.back() != '/') killtrmpth.pop_back();
    killtrmpth += "../share/factoryrush/bin/killterm";
    fwrite(killtrmpth.c_str(), sizeof(char), killtrmpth.size(), file);
    fwrite("\n", sizeof(char), 1, file);
    fchmod(fileno(file), 0755);
    fclose(file);
    if (openFileInApp(runpth.c_str(), term.c_str()) != 0) return nullopt;
#endif
#ifdef _WIN32
    goto contcons;
console:
    for (int i = 0; i < argc; i++) args[i] = argv[i];
    args[argc] = info.c_str();
    args[argc+1] = nullptr;
    if (!System::RunProgramAsyncC(root_pth.c_str(), args)) {
        ResumeThread(Hinput_thread);
        return nullopt;
    }
contcons:
    if (!tabby2)
#endif
    System::FreeMemory(args);

    // maybe don't wait or do it on a thread [?]
    Console::out << L"Waiting for popup to start until: " << max_popup_startup_wait; out_endl();
    auto start = chrono::high_resolution_clock::now();
    while ((!System::IsFile((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str())) && (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < max_popup_startup_wait)) {
    #ifdef _WIN32
        if (tabby && tabby2 < 5 && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() + 1000 > (max_popup_startup_wait/3)*(max(min(2,tabby2),1)) && System::IsFile((Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd").c_str())) {
            Console::out << L"Try again " << ++tabby2; out_endl();
            goto runtabby;
        }
    #endif
        SysSleep(1000);
        Console::out << L"Waiting for popup to start: " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count(); out_endl();
    }
    if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() >= max_popup_startup_wait) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    Console::out << L"Popup launched successfully" << L'\n';
    Console::HandleOutput();
    FILE* fl = topen((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str(), N("r"));
    if (!fl) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    pid_t spid = 0;
    fread(&spid, sizeof(pid_t), 1, fl);
    fclose(fl);
    Console::popup_pids.push_back(spid);

    auto& pop_pids = Console::popup_pids;
    auto retx = stsb::promise<std::optional<std::pair<int, uniconv::utfstr>>>();
    stsb::promise<int> tstint = stsb::promise<int>();
    auto retdir = Console::tmp_data + subdir + procdir;
    thread waitthr = thread([retx, retdir, tstint, pop_pids, spid]() mutable {
        bool oldfocus = false;
        while (!System::IsFile((retdir + N("exit.dat")).c_str())) SysSleep(1000);

        for (size_t i = 0; i < popup_pids.size(); i++)
            if (popup_pids[i] == spid) { popup_pids.erase(popup_pids.begin() + i); break; }

        FILE* fl = topen((retdir + N("exit.dat")).c_str(), N("r"));
        if (!fl) ThrowMsg(utfstr(N("Couldn't open file: ")) + retdir + N("exit.dat"));
        int pret = 0;
        fread(&pret, sizeof(int), 1, fl);
        fclose(fl);

        utfstr result;
        fl = topen((retdir + N("result.dat")).c_str(), N("r"));
        if (!fl) return nullopt;

        unsigned long long len = 0;
        fread(&len, sizeof(unsigned long long), 1, fl);
        if (len) {
            char_t* buf = new char_t[len+1];
            fread(buf, sizeof(char_t), len, fl);
            buf[len] = 0;
            result = buf;
            delete[] buf;
        }

        System::RemoveFile((retdir + N("pid.dat")).c_str());
        System::RemoveFile((retdir + N("exit.dat")).c_str());
        System::RemoveFile((retdir + N("result.dat")).c_str());

        System::DeleteDirectory(retdir.c_str());

        if (pret)
            Console::out << L"Popup exited with code: 0x" << std::hex << pret << L" and result: \"" << NativeToWString(result) << L"\"\n";
        
        retx = std::optional<std::pair<int, uniconv::utfstr>>({pret,result});
        return nullopt;
    });
    waitthr.detach();
#ifdef _WIN32
    ResumeThread(Hinput_thread);
#endif
    return retx;
}

std::optional<stsb::promise<std::optional<std::pair<int, std::u16string>>>> cpp::Console::PopupWindowAsync(int type, int argc, const char16_t *arg16v[], const char16_t *u16title) {
    vector<utfstr> argv;
    argv.reserve(argc);
    for (int i = 0; i < argc; i++)
        argv.push_back(UnicodeToNativeString(U16StringToUnicode(arg16v[i])));
    auto strtitle = u16title ? UnicodeToNativeString(U16StringToUnicode(u16title)) : utfstr();
    auto title = u16title ? strtitle.c_str() : nullptr;
    
    auto term = Console::GetTerminalExecutableName();

#ifdef _WIN32
    HANDLE Hinput_thread = Console::thread_handles->front(); // input thread was added first
    SuspendThread(Hinput_thread);
#endif

    int pc = 0;
newpidgen:
    if (++pc > UINT16_MAX) exit(0xF1); // no pid left
    if (Console::next_pid == UINT16_MAX) Console::next_pid = 0;
    auto& npid = ++Console::next_pid;
    if (Console::used_pids[npid]) goto newpidgen;
    Console::used_pids[npid] = true;

    utfcstr* args = (utfcstr*)System::AllocateMemory(sizeof(void*) * (argc+3
#ifdef _WIN32
    + conemu*7 // conemu need 7 more args
    - cmder // cmder uses one less arg than conemu
#else
    + tabby // tabby need 1 more arg
#endif
    ));

#if defined(_WIN32) || defined(__CYGWIN__)
    #define sep L'\\'
    #define topen _wfopen
    #define fgetnc fgetwc
#else
    #define sep '/'
    #define topen fopen
    #define fgetnc fgetc
    #define ERROR_PATH_NOT_FOUND ENOENT

    char* appdata = getenv("HOME");
    const char* tmppath = "/tmp";
#endif
    utfstr procdir = utfstr(N("proc")) + sep + to_nstring(npid) + sep;
    if (System::MakeDirectory((Console::tmp_data + subdir + procdir).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::tmp_data + sep + subdir + procdir + N("\"")).c_str());
    if (System::MakeDirectory((Console::dev_data + N("logs") + sep + to_nstring(type) + sep).c_str()) == ERROR_PATH_NOT_FOUND) ThrowMsg((utfstr(N("Couldn't create directory: \"")) + Console::dev_data + sep + N("logs") + sep + to_nstring(type) + sep + N("\"")).c_str());
    utfstr info = N("\033&");
    info.append(to_nstring(type)).push_back(N('~'));
    info.append(subdir).append(procdir).push_back(N('~'));
    utfstr root_pth = System::GetSelfPath();
#ifdef __linux__
    auto termchk = term;
#else
    // ".exe", ".app"
    auto termchk = term; termchk.pop_back(); termchk.pop_back(); termchk.pop_back(); termchk.pop_back();
#endif
    // Hyper doesn't have a cli option to run a program, so we have to use the default term
    if (termchk[termchk.size()-1] == N('r') && termchk[termchk.size()-2] == N('e') && termchk[termchk.size()-3] == N('p') && termchk[termchk.size()-4] == N('y') && termchk[termchk.size()-5] == N('H'))
        term.clear();

#ifdef _WIN32
    wstring term2;
    wstring newconstitle, newconsicon, newwinicon;
    int tabby2 = 0;

    if (mintty && !conemu && !good) {
        if (term.find(L"mintty") == std::wstring::npos) {
            Console::window = GetForegroundWindow();
        }
        term = Console::GetTerminalExecutableName();
        if (!(good = term.find(L"mintty") != std::wstring::npos))
            goto console;
    } else if (!term.size())
        goto console;
#elif __CYGWIN__
    if (!term.size()) {
        term.append(L"C:\\cygwin64\\bin\\mintty.exe"); // todo - if mintty doesn't exists the use default 
        if (false) goto console;
    }
#elif __APPLE__
    if (!term.size()) {
        term.append("/System/Applications/Utilities/Terminal.app"); // idk if this is the actual path
    }
#elif __linux__
    if (!term.size()) {
        term.append("/usr/bin/gnome-terminal"); // todo - find default terminal emulator
    }
#else
    if (!term.size()) {
        return nullopt;
    }
#endif
    args[0] = root_pth.c_str();
    args[1] = info.c_str();
    for (int i = 1; i <= argc; i++) args[i+1] = argv[i-1].c_str();
    args[argc+2] = nullptr;
#ifdef _WIN32 // for some reason, WindowsTerminal.exe doesn't work, but wt.exe does
    if (wt) {
        term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"wt.exe");
        if (System::RunProgramAsync(term2.c_str(), args)) goto contcons;
    }
    if (tabby) {
        /* need to close the window after the popup exits
         * so this creates a powershell script that run's the popup and kill the parent process
         * and creates a batch file that runs the powershell script
         * than runs the `term` to create a new window
         * and after launch runs the batch file with the `term`
        */
       {
            wstring runps = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".ps1";
            auto file = _wfopen(runps.c_str(), L"w");
            _setmode(_fileno(file),  _O_WTEXT); wint_t x = 0xfeff; fwrite(&x, sizeof(wint_t), 1, file);

            fwrite(L"del ($MyInvocation.MyCommand).Path\ndel \"$(Split-Path -Parent -Path $($MyInvocation.MyCommand).Path)\\", sizeof(wchar_t), 100, file);
            fwrite(to_wstring(npid).c_str(), sizeof(wchar_t), to_wstring(npid).size(), file);
            fwrite(L".cmd\"\n& ", sizeof(wchar_t), 8, file);
            for (int i = 0; args[i]; i++)
                fwprintf(file, L"\"%ls\" ", args[i]);
            fwprintf(file, L"\n%ls\\bin\\killwindow.exe", System::GetRootDir().c_str());
            fclose(file);

            wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
            file = _wfopen(runpth.c_str(), L"w");
            fwrite("@echo off\npowershell -NoProfile -ExecutionPolicy Bypass -Command \"%~dp0", sizeof(char), 71, file);
            fwrite(to_string(npid).c_str(), sizeof(char), to_string(npid).size(), file);
            fwrite(".ps1\"\n", sizeof(char), 5, file);
            fclose(file);

            if (!System::RunProgramAsync(term.c_str(), (const wchar_t*)nullptr)) {
                ResumeThread(Hinput_thread);
                return nullopt; // Run to launch new window
            }
            DWORD processId;
            GetWindowThreadProcessId(Console::window, &processId);
            unordered_map<DWORD, bool> processes;
            {
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    processes[prc] = true;
                }
            }
            while (1) {
                SysSleep(1000);
                auto prcs = GetChildProcessIds(processId);
                for (auto&& prc : prcs) {
                    if (processes[prc]) continue;
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
                    if (processHandle) {
                        wchar_t exePath[MAX_PATH];
                        if (GetModuleFileNameEx(processHandle, NULL, exePath, MAX_PATH)) {
                            Console::out << L"New child process: " << exePath; out_endl();
                            tabby = false;
                            ModifyExecutableName(exePath);
                            if (tabby) {
                                Console::out << L"Tabby process\n"; out_endl();
                                auto thiswin = GetForegroundWindow();
                                while (thiswin == GetForegroundWindow()) SysSleep(1000);
                                
                                Console::Sleep(0.9);
                                goto runtabby;
                            } else tabby = true;
                        }
                    }
                    processes[prc] = true;
                }
            }
        }
    runtabby:
        wstring runpth = Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd";
        const wchar_t** realargs = (const wchar_t**)System::AllocateMemory(sizeof(void*) * (5));
        realargs[0] = L"run";
        realargs[1] = runpth.c_str();
        realargs[2] = L";";
        realargs[3] = L"exit";
        realargs[4] = nullptr;
        Console::out << L"Running: " << term << L" " << runpth; out_endl();
        if (!System::RunProgramAsync(term.c_str(), realargs)) {
            ResumeThread(Hinput_thread);
            return nullopt;
        }
        Console::out << L"Started: " << term << L" " << runpth; out_endl();
        System::FreeMemory(realargs);
        goto contcons;
    }
    else if (conemu) {
        for (int i = argc+2+7-cmder; i >= 7-cmder; i--) args[i] = args[i-(7-cmder)]; // cmder desn't need args[6]
        args[0] = L"-Title";
        args[1] = title ? title : Console::window_title.c_str();
        args[2] = L"-cmd"; // new versions also support "-run", but old ones don't
        // (when new vesions stop supporting '-cmd' I'll have to get the version somehow)
        newconstitle = L"-new_console:t:"; newconstitle.append(args[1]);
        args[3] = newconstitle.c_str();
        newconsicon = L"-new_console:C:"; newconsicon.append(System::GetRootDir()).append(L"\\assets\\images\\icon.ico");
        args[4] = newconsicon.c_str();
        if (cmder) {
            term2 = term; while (term2.back() != '\\') term2.pop_back(); term2.append(L"vendor\\git-for-windows\\usr\\bin\\mintty.exe");
            args[5] = term2.c_str();
            wstring cmdargs;
            for (int i = 0; i < argc + 2 + 7 - 1; i++)
                cmdargs.append(L"\"").append(args[i]).append(L"\" ");
            cmdargs.pop_back();
            const wchar_t* realargs[3] = { L"/x", cmdargs.c_str(), nullptr };
            if (!System::RunProgramAsync(term.c_str(), realargs)) {
                ResumeThread(Hinput_thread);
                return nullopt;
            }
            goto contcons;
        } else {
            args[6] = args[4];
            args[5] = args[3];
            args[4] = args[2];
            args[2] = L"-icon";
            newwinicon = (System::GetRootDir() + L"\\assets\\images\\icon.ico");
            args[3] = newwinicon.c_str();
        }
    }
#else
    if (tabby) {
        for (int i = argc+2; i > 0; i--) args[i] = args[i-1];
        args[0] = L"run";
    }
#endif
#ifndef __APPLE__
    if (!System::RunProgramAsync(term.c_str(), args)) {
#ifdef _WIN32
        ResumeThread(Hinput_thread);
#endif
        return nullopt;
    }
#else
    string runpth = string("/tmp/.factoryrush/") + subdir + "proc/" + to_nstring(npid) + ".command";
    auto file = fopen(runpth.c_str(), "w");
    fwrite("#!/bin/sh\nrm -f $0\n", sizeof(char), 19, file);
    for (int i = 0; args[i]; i++)
        { fwrite("\"",sizeof(char),1,file); fwrite(args[i], sizeof(char), strlen(args[i]), file); fwrite("\" ", sizeof(char), 2, file); }
    fwrite("\n", sizeof(char), 1, file);
    string killtrmpth = root_pth; while (killtrmpth.back() != '/') killtrmpth.pop_back();
    killtrmpth += "../share/factoryrush/bin/killterm";
    fwrite(killtrmpth.c_str(), sizeof(char), killtrmpth.size(), file);
    fwrite("\n", sizeof(char), 1, file);
    fchmod(fileno(file), 0755);
    fclose(file);
    if (openFileInApp(runpth.c_str(), term.c_str()) != 0) return nullopt;
#endif
#ifdef _WIN32
    goto contcons;
console:
    for (int i = 0; i < argc; i++) args[i] = argv[i].c_str();
    args[argc] = info.c_str();
    args[argc+1] = nullptr;
    if (!System::RunProgramAsyncC(root_pth.c_str(), args)) {
        ResumeThread(Hinput_thread);
        return nullopt;
    }
contcons:
    if (!tabby2)
#endif
    System::FreeMemory(args);

    // maybe don't wait or do it on a thread [?]
    Console::out << L"Waiting for popup to start until: " << max_popup_startup_wait; out_endl();
    auto start = chrono::high_resolution_clock::now();
    while ((!System::IsFile((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str())) && (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < max_popup_startup_wait)) {
    #ifdef _WIN32
        if (tabby && tabby2 < 5 && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() + 1000 > (max_popup_startup_wait/3)*(max(min(2,tabby2),1)) && System::IsFile((Console::tmp_data + subdir + L"proc\\" + to_wstring(npid) + L".cmd").c_str())) {
            Console::out << L"Try again " << ++tabby2; out_endl();
            goto runtabby;
        }
    #endif
        SysSleep(1000);
        Console::out << L"Waiting for popup to start: " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count(); out_endl();
    }
    if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() >= max_popup_startup_wait) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    Console::out << L"Popup launched successfully" << L'\n';
    Console::HandleOutput();
    FILE* fl = topen((Console::tmp_data + subdir + procdir + N("pid.dat")).c_str(), N("r"));
    if (!fl) {
    #ifdef _WIN32
        ResumeThread(Hinput_thread);
    #endif
        return nullopt;
    }
    pid_t spid = 0;
    fread(&spid, sizeof(pid_t), 1, fl);
    fclose(fl);
    Console::popup_pids.push_back(spid);

    auto& pop_pids = Console::popup_pids;
    auto retx = stsb::promise<std::optional<std::pair<int, std::u16string>>>();
    stsb::promise<int> tstint = stsb::promise<int>();
    auto retdir = Console::tmp_data + subdir + procdir;
    thread waitthr = thread([retx, retdir, tstint, pop_pids, spid]() mutable {

        bool oldfocus = false;
        while (!System::IsFile((retdir + N("exit.dat")).c_str())) SysSleep(1000);

        for (size_t i = 0; i < popup_pids.size(); i++)
            if (popup_pids[i] == spid) { popup_pids.erase(popup_pids.begin() + i); break; }

        FILE* fl = topen((retdir + N("exit.dat")).c_str(), N("r"));
        if (!fl) ThrowMsg(utfstr(N("Couldn't open file: ")) + retdir + N("exit.dat"));
        int pret = 0;
        fread(&pret, sizeof(int), 1, fl);
        fclose(fl);

        utfstr result;
        fl = topen((retdir + N("result.dat")).c_str(), N("r"));
        if (!fl) return nullopt;
        unsigned long long len = 0;
        fread(&len, sizeof(unsigned long long), 1, fl);
        if (len) {
            char_t* buf = new char_t[len+1];
            fread(buf, sizeof(char_t), len, fl);
            buf[len] = 0;
            result = buf;
            delete[] buf;
        }

        System::RemoveFile((retdir + N("pid.dat")).c_str());
        System::RemoveFile((retdir + N("exit.dat")).c_str());
        System::RemoveFile((retdir + N("result.dat")).c_str());

        System::DeleteDirectory(retdir.c_str());

        if (pret)
            Console::out << L"Popup exited with code: 0x" << std::hex << pret << L" and result: \"" << NativeToWString(result) << L"\"\n";
        
        retx = std::optional<std::pair<int, u16string>>({pret,UnicodeToU16String(NativeStringToUnicode(result.c_str()))});
        return nullopt;
    });
    waitthr.detach();
#ifdef _WIN32
    ResumeThread(Hinput_thread);
#endif
    return { retx };
}

void Console::DontHandleKeyboard(void) {
    Console::key_hit = -1;
    Console::key_released = -1;
}

void Console::ResetKeyboard(void) {
    Console::key_hit = -1;
    Console::key_released = -1;
    for (int i = 0; i < KEYBOARD_MAX; i++)
        Console::key_states[i] = false;
}

void Console::Update(void) {
    Console::HandleKeyboard();
    Console::HandleMouseAndFocus();
    Console::HandleOutput();
}
