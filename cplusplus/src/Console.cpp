#include "Console.hpp"

using namespace cpp;
using namespace uniconv;
using namespace std;
using namespace std::chrono;

#ifdef _WIN32
#include <iostream>

    inline uint8_t Console::GenerateAtrVal(uint8_t i1, uint8_t i2) {
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
    inline pair<uint8_t,uint8_t> AtrValToColors(uint8_t val) {
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

    HANDLE Console::screen = HANDLE();
    HWND Console::window = HWND();
    HDC Console::device = HDC();
    uint8_t Console::default_fcol = uint8_t();
    uint8_t Console::default_bcol = uint8_t();
    pair<uint16_t,uint16_t> Console::xyoffset = pair<uint16_t,uint16_t>();

    inline HWND GetHwnd(void) {
        HWND hwndFound;
        wchar_t pszNewWindowTitle[1024];
        wsprintf(pszNewWindowTitle,L"%d/%d",GetTickCount(),GetCurrentProcessId());
        SetConsoleTitle(pszNewWindowTitle);
        SysSleep(40e3);
        hwndFound=FindWindow(NULL, pszNewWindowTitle);
        return(hwndFound);
    }

    inline pair<uint16_t,uint16_t> Console::GetXYCharOffset() {
        return pair<uint16_t,uint16_t>(0,0);
    #define GetXYCharOffset_MaxXSearch 25
    #define GetXYCharOffset_MaxYSearch 50
        auto scr = vector<vector<Symbol>>();
        scr.push_back(vector<Symbol>());
        scr.back().push_back(Console::Symbol(L' ', 1, 1));
        FillScreen(scr);
        CONSOLE_SCREEN_BUFFER_INFOEX csbix;
        GetConsoleScreenBufferInfoEx(Console::screen, &csbix);
        COLORREF red = csbix.ColorTable[4];
        cerr << csbix.ColorTable[0] << ' ' << csbix.ColorTable[1] << ' ' << csbix.ColorTable[2] << ' ' << csbix.ColorTable[3] << ' ' << csbix.ColorTable[4] << '\n';
        
        auto out = pair<uint16_t,uint16_t>(-1,-1);
        for (uint16_t i = 0; i < GetXYCharOffset_MaxXSearch; i++) {
            for (uint16_t j = 0; j < GetXYCharOffset_MaxYSearch; j++) {
                POINT p = {i, j};
                ClientToScreen(Console::window,&p);
                COLORREF col = GetPixel(Console::device, p.x, p.y);
                if (red == col) {
                    out.first = i;
                    out.second = j;
                    goto GetXYCharOffsetOutOfLoop;
                }
            }
        }
GetXYCharOffsetOutOfLoop:
        cerr << '\n' << '\n' << out.first << ' ' << out.second << '\n';
        return out;
    }

    void Console::Init(void) {
        if (!initialised) {

            Console::screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            SetConsoleActiveScreenBuffer(Console::screen);

            Console::window = GetHwnd();
            Console::device = GetDC(Console::window);
            Console::xyoffset = Console::GetXYCharOffset();

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(Console::screen, &csbi);
            auto val = AtrValToColors(csbi.wAttributes);
            Console::default_fcol = val.first;
            Console::default_bcol = val.second;
            
            atexit(Console::Fin);
            at_quick_exit(Console::Fin);

            signal(SIGINT, quick_exit);
            signal(SIGILL, quick_exit);
            signal(SIGABRT, quick_exit);
            signal(SIGFPE, quick_exit);
            signal(SIGSEGV, quick_exit);
            signal(SIGTERM, quick_exit);
            signal(SIGBREAK, quick_exit);
            
            initialised = true;
        }
    }

    void Console::Fin(void) {
        if (initialised) {
            SetConsoleActiveScreenBuffer((HANDLE)nullptr);

            initialised = false;
        }
    }

    int16_t cpp::Console::GetWindowWidth(void) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(screen, &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    int16_t cpp::Console::GetWindowHeight(void) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(screen, &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    array<unsigned long, 2> Console::FillScreen(vector<vector<Symbol> > symbols) {
        Symbol empty_sym = Symbol(L' ', 16, 16);
        const size_t win_width = GetWindowWidth(), win_height = GetWindowHeight(), height = symbols.size(), width = height > 0 ? symbols[0].size() : 0;

        wchar_t* screen = new wchar_t[win_height*win_width];
        WORD* attributes = new WORD[win_height*win_width];

		for (size_t i = 0; i < win_height; i++) {
			for (size_t j = 0; j < win_width; j++) {
                if (i >= height || j >= width) {
                    screen[ i*width + j ] =  empty_sym.character;
                    attributes[ i*width + j ] = empty_sym.GetAttribute();
                    continue;
                }
                screen[ i*width + j ] = symbols[i][j].character;
                attributes[ i*width + j ] = symbols[i][j].GetAttribute();
			}
		}

        array<DWORD,2> written;
		BOOL out = WriteConsoleOutputCharacter(Console::screen, screen, width*height, { 0,0 }, &(written[0]) );
        if (out == 0) { exit(GetLastError()); }
		out = WriteConsoleOutputAttribute(Console::screen, attributes, width*height, { 0,0 }, &(written[1]) );
        if (out == 0) { exit(GetLastError()); }
        
		delete[] screen;
		delete[] attributes;

        return written;
    }

    void Console::HandleKeyboard() {
        key_hit = -1;
        key_released = -1;
        for (int i = 0; i < KEYBOARD_MAX; i++) {
            SHORT keyState = GetKeyState(i);
            if (!key_states[i] && keyState & 0x8000) key_hit = i;
            if (key_states[i] && !(keyState & 0x8000)) key_released = i;
            
            key_states[i] = keyState & 0x8000;
            key_states[KEYBOARD_MAX + i] = keyState & 1;
        }
    }

    void Console::HandleMouseAndFocus(void) {
        Console::focused = (Console::window == GetForegroundWindow());

        POINT p;
        GetCursorPos(&p);
        ScreenToClient(Console::window, &p);
        Console::mouse_status.x = (p.x < 0) ? Console::mouse_status.x : p.x;
        Console::mouse_status.y = (p.y < 0) ? Console::mouse_status.y : p.y;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(Console::screen, &csbi);

        cerr << ' ' << csbi.dwSize.X << ' ' << csbi.dwSize.Y << '\n';

        return;
    }

    void SysSleep(int microseconds){
        Sleep((int)(microseconds/1000));
    }

    uint8_t Console::Symbol::GetAttribute(void) {
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
#else
// Not linux (Probably Posix and Unix)


#ifdef __linux__
// linux
    string GenerateEscapeSequence(uint8_t,uint8_t);

    inline int parse_input(int show_keycodes, char * buf, int n) {
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

    void Console::Init(void) {
        if (!initialised) {

            int err;

            fd = getfd();

            if (fd < 0) {
                char buf[256];
                int buf_size = readlink( "/proc/self/exe" , buf, 256);
                string command = "sudo ";
                command.append(buf);
                exit(system(command.c_str()));
                
                string error = GenerateEscapeSequence(1,16) + "\nCouldn't get a file descriptor referring to the console.\nCheck if you have acces to /dev/tty and /dev/console.\n" + GenerateEscapeSequence(4,16) + "\n\ttry: " + GenerateEscapeSequence(6,16) + "sudo " + buf + "\033[0m\n";
                throw(runtime_error(error.c_str()));
            }

            char path[256];
            int path_size = readlink( "/proc/self/exe" , path, 256);
            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode.bin ");
            command.push_back('0' + K_MEDIUMRAW);

            if (err = ioctl(fd, KDGKBMODE, &old_kbdmode)) {
                throw("ioctl KDGKBMODE error");
            }

            if (system(command.c_str()) < 0) {
                throw("setkbdmode.bin error");
            }
            
            fwrite("\033[?1049h", sizeof(char), 8, stderr);
            
            fwrite("\033[?1004h", sizeof(char), 8, stderr);

            fwrite("\033[?1003h", sizeof(char), 8, stderr);
            fwrite("\033[?1015h", sizeof(char), 8, stderr);
            fwrite("\033[?1006h", sizeof(char), 8, stderr);

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
            
            //ioctl(fd, KDSETMODE, old_kbdmode);
            tcsetattr(fd,TCSANOW,&old_fdterm);

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);
            
            fwrite("\033[?1049l", sizeof(char), 8, stderr);
            
            fwrite("\033[?1004l", sizeof(char), 8, stderr);

            fwrite("\033[?1003l", sizeof(char), 8, stderr);
            fwrite("\033[?1015l", sizeof(char), 8, stderr);
            fwrite("\033[?1006l", sizeof(char), 8, stderr);

            char path[256];
            int path_size = readlink( "/proc/self/exe" , path, 256);
            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode.bin ");
            command.push_back('0' + old_kbdmode);
            if (old_kbdmode = system(command.c_str()) < 0) {
                throw("setkbdmode.bin error");
            }

            initialised = false;
        }
    }

    void Console::HandleMouseAndFocus(void) {
        Console::this_mouse_button = -1;
        Console::this_mouse_down = false;
        Console::this_mouse_combo = 0;
        Console::mouse_buttons_down[3] = false;
        Console::mouse_buttons_down[4] = false;

        bool mousedown;
        int bytes;

        ioctl(fileno(stdin), FIONREAD, &bytes);

        if (bytes < 3) {
            return;
        }

        if (getc(stdin) != '\e') return;
        if (getc(stdin) != '[') return;
        if (bytes == 3) {
            switch (getc(stdin)) {
            case 'I':
                Console::focused = true;
                return;
            case 'O':
                Console::focused = false;
                return;
            default:
                return;
            }
        }
        if (getc(stdin) != '<') return;
        bytes -= 3;
        while (bytes > 6) {
            int pos = 0;
            int val[3] = {0};
            while (bytes > 0) {
                char byte = getc(stdin); --bytes;

                if (byte == ';') { ++pos; continue; }
                
                if (byte == 'M') { mousedown = true; break; }
                if (byte == 'm') { mousedown = false; break; }

                int num = byte - '0';
                if (num < 0 || num > 9) return;

                val[pos] *= 10;
                val[pos] += num;
            };

            Console::mouse_status.x = val[1];
            Console::mouse_status.y = val[2];

            bitset<8> event(val[0]);

            if (event[5]) {
                return; // move event
            }
            
            uint8_t button = 0b0;
                
            button += 0b1 * event[0];
            button += 0b10 * event[1];

            chrono::time_point<chrono::high_resolution_clock> now = chrono::high_resolution_clock::now();

            uint8_t fullbutton = 0b000000;
            fullbutton |= 0b000001 * button; // BBB
            fullbutton |= 0b001000 * event[2]; // S
            fullbutton |= 0b010000 * event[3]; // M
            fullbutton |= 0b100000 * event[4]; // C
            // fullbutton == 0bCMSBBB

            if (mousedown) {
                if (Console::last_mouse_button == fullbutton && now - Console::last_click_time <= chrono::milliseconds(Console::double_click_max)) {
                    if (Console::last_mouse_combo < UINT8_MAX) ++Console::last_mouse_combo;
                } else {
                    Console::last_mouse_button = (button < 3) ? fullbutton : -1;
                    Console::last_mouse_combo = 1;
                }
                Console::last_click_time = now;
            }

            Console::this_mouse_combo = ( Console::last_click_time == now ) ? Console::last_mouse_combo : 1;
            Console::this_mouse_down = mousedown;
            Console::mouse_buttons_down[fullbutton] = this_mouse_down;
            Console::this_mouse_button = fullbutton;
        }
        return;
    }

    void Console::HandleKeyboard(void) {
        int bytes;
        char buf[16];
        //ioctl(fileno(stdin), FIONREAD, &bytes);

        bytes = read(fd, buf, sizeof(buf));

        if (bytes <= 0) {
            return;
        }

        int parsed = parse_input(true,buf,bytes);

        key_hit = -1;
        key_released = -1;
        if ( !key_states[parsed % KEYBOARD_MAX] && (parsed / KEYBOARD_MAX) ) key_hit = parsed % KEYBOARD_MAX;
        if ( key_states[parsed % KEYBOARD_MAX] && !(parsed / KEYBOARD_MAX) ) key_released = parsed % KEYBOARD_MAX;
        if ( key_hit > 0 ) key_states[KEYBOARD_MAX + key_hit] = !key_states[KEYBOARD_MAX + key_hit];
        key_states[parsed % KEYBOARD_MAX] = !(parsed / KEYBOARD_MAX);

        return;
    }

    struct termios Console::old_fdterm = termios();
    int Console::old_kbdmode = int();

#elif __APPLE__
// macOS
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

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);
            
            fwrite("\033[?1049l",sizeof(char), 8, stderr);

            initialised = false;
        }
    }
#else
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

            tcsetattr(STDIN_FILENO,TCSANOW,&old_termios);
            
            fwrite("\033[?1049l",sizeof(char), 8, stderr);

            initialised = false;
        }
    }
#endif

// all non windows
    inline string GenerateEscapeSequence(uint8_t i1, uint8_t i2) {
        string val = "\033[";
        if (i1 < 8) {
            val.append(to_string(30 + i1));
        } else if (i1 < 16) {
            val.append(to_string(90 + i1));
        } else {
            val.append("39");
        }
        val.push_back(';');
        if (i2 < 8) {
            val.append(to_string(40 + i2));
        } else if (i2 < 16) {
            val.append(to_string(100 + i2));
        } else {
            val.append("49");
        }
        val.push_back('m');
        return val;
    }


    int16_t cpp::Console::GetWindowWidth(void) {
        struct winsize size;
        ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
        return size.ws_col;
    }

    int16_t cpp::Console::GetWindowHeight(void) {
        struct winsize size;
        ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
        return size.ws_row;
    }

    array<unsigned long,2> Console::FillScreen(vector<vector<Console::Symbol> > symbols) {
        string screen = "\e[H";
        size_t width = GetWindowWidth(), height = GetWindowHeight();
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                if (i >= symbols.size() || j >= symbols[0].size()) {
                    screen.append("\033[0m ");
                    continue;
                }
                screen.append( GenerateEscapeSequence(symbols[i][j].foreground, symbols[i][j].background) );
                screen.append(symbols[i][j].character);
            }
        }
        screen.append("\033[0m");
        
        //FILE * screen_file = fopen( "screen.dat", "w" );
        fwrite(screen.c_str(), sizeof(char), screen.size(), stderr);
        //system("cat screen.dat");

        array<unsigned long,2> written;
				
		written[0] = width*height;
        written[1] = width*height + 1;

        return written;
    }

    void SysSleep(int microseconds){
        int ret;
        if (!microseconds % 1000000) ret = 1000000 * sleep(microseconds / 1000000);
        else ret = usleep(microseconds);
        if (ret) SysSleep(ret);
    }

    struct termios Console::old_termios = termios();
    int Console::fd = int();
#endif

bool Console::initialised = false;
bitset<KEYBOARD_MAX*2> Console::key_states = bitset<KEYBOARD_MAX*2>(0);
bitset<5> Console::mouse_buttons_down = bitset<5>(0);
struct Console::MouseStatus Console::mouse_status = Console::MouseStatus();
std::chrono::time_point<std::chrono::high_resolution_clock> Console::last_click_time = std::chrono::high_resolution_clock::now();

int Console::key_hit = int();
int Console::key_released = int();
uint8_t Console::this_mouse_button = -1;
uint8_t Console::this_mouse_combo = 0;
bool Console::this_mouse_down = false;
uint8_t Console::last_mouse_button = -1;
uint8_t Console::last_mouse_combo = 0;
bool Console::focused = true;
unsigned short Console::double_click_max = 500;

bool Console::IsKeyDown(int key) {
    return Console::key_states[key];
}

bool Console::IsKeyToggled(int key) {
    return Console::key_states[KEYBOARD_MAX + key];
}

int Console::KeyPressed(void) {
    return Console::key_hit;
}

int Console::KeyReleased(void) {
    return Console::key_released;
}

bool Console::IsFocused(void) {
    return Console::focused;
}

struct Console::MouseStatus Console::GetMouseStatus(void) {
    return Console::mouse_status;
}

pair<uint8_t, uint8_t> Console::MouseButtonClicked(void) {
    return Console::this_mouse_down ? pair<uint8_t, uint8_t>(-1,0) : pair<uint8_t, uint8_t>(Console::this_mouse_button, Console::this_mouse_combo);
}

uint8_t cpp::Console::MouseButtonReleased(void) {
    return Console::this_mouse_down ? Console::this_mouse_button : -1;
}

void Console::Sleep(double seconds) {

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
}

void cpp::Console::SetDoubleClickMaxWait(unsigned short milliseconds) {
    Console::double_click_max = milliseconds;
}

unsigned short cpp::Console::GetDoubleClickMaxWait(void) {
    return Console::double_click_max;
}

Console::MouseStatus::MouseStatus(void) {
    this->primary = false;
    this->middle = false;
    this->secondary = false;
    this->scroll = pair<bool,bool>(false,false);
    this->x = -1;
    this->y = -1;
}

Console::Symbol::Symbol(utfchar character, uint8_t foreground, uint8_t background) {
    this->character = character;
    this->foreground = foreground;
    this->background = background;
}

Console::Symbol::Symbol(void) {
    this->character = WCharToNative(L' ');
    this->foreground = 7;
    this->background = 0;
}

Console::Symbol::Symbol(const Symbol& sym) {
    this->character = sym.character;
    this->foreground = sym.foreground;
    this->background = sym.background;
}

Console::Symbol::~Symbol(void) {
    this->character = -1;
    this->foreground = -1;
    this->background = -1;
}

Console::Symbol & Console::Symbol::operator=(const Console::Symbol & src) {
    if (this == &src) { return *this; }
    this->character = src.character;
    this->background = src.background;
    this->foreground = src.foreground;
    return *this;
}