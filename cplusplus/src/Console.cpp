#include "Console.hpp"

using namespace cpp;
using namespace uniconv;
using namespace std;
using namespace std::chrono;

#ifdef _WIN32
#include <iostream>
    inline uint8_t GenerateAtrVal(uint8_t i1, uint8_t i2) {
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
        if (i1 >= 16) { val |= 0x0000; }
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
        if (i2 >= 16) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; }
        return val;
    }

    HANDLE Console::h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    HWND Console::win_console = GetHwnd();

    HWND Console::GetHwnd(void) {
        #define MY_BUFSIZE 1024
        HWND hwndFound;
        wchar_t pszNewWindowTitle[MY_BUFSIZE];
        wchar_t pszOldWindowTitle[MY_BUFSIZE];

        GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

        wsprintf(pszNewWindowTitle,L"%d/%d",GetTickCount(),GetCurrentProcessId());
        SetConsoleTitle(pszNewWindowTitle);
        Sleep(40);

        hwndFound=FindWindow(NULL, pszNewWindowTitle);
        
        SetConsoleTitle(pszOldWindowTitle);

        return(hwndFound);
    }

    void Console::Init(void) {
        if (!initialised) {
            
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


            SetConsoleActiveScreenBuffer(Console::h_console);

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
        GetConsoleScreenBufferInfo(h_console, &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    int16_t cpp::Console::GetWindowHeight(void) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(h_console, &csbi);
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
		BOOL out = WriteConsoleOutputCharacter(h_console, screen, width*height, { 0,0 }, &(written[0]) );
        if (out == 0) { exit(GetLastError()); }
		out = WriteConsoleOutputAttribute(h_console, attributes, width*height, { 0,0 }, &(written[1]) );
        if (out == 0) { exit(GetLastError()); }
        
		delete[] screen;
		delete[] attributes;

        return written;
    }

    int Console::HandleKeyboard() {
        int out = -1;
        key_hit = -1;
        key_released = -1;
        for (int i = 0; i < KEYBOARD_MAX; i++) {
            SHORT keyState = GetKeyState(i);
            if (!key_states[i] && keyState & 0x8000) key_hit = i;
            if (!key_states[i] && keyState & 0x8000) out = i;
            if (key_states[i] && !(keyState & 0x8000)) key_released = i;
            if (key_states[i] && !(keyState & 0x8000)) out = KEYBOARD_MAX +  i;
            
            key_states[i] = keyState & 0x8000;
            key_states[KEYBOARD_MAX + i] = keyState & 1;
        }
        return out;
    }

    void SysSleep(int microseconds){
        Sleep((int)(microseconds/1000));
    }

    uint8_t Console::Symbol::GetAttribute(void) {
        return GenerateAtrVal(this->foreground,this->background);
    }

    void Console::Symbol::SetAttribute(uint8_t attribute) {
        this->foreground = 7;
        this->background = 0;
    }

    Console::Symbol::Symbol(uint8_t attribute) {
    this->character = L' ';
    this->SetAttribute(attribute);
}
#else
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
            
            fwrite("\033[?1049h",sizeof(char), 8, stderr);

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
            
            fwrite("\033[?1049l",sizeof(char), 8, stderr);

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

    int parse_input(int show_keycodes, char * buf, int n) {
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

    int cpp::Console::HandleKeyboard(void) {
        int bytes;
        char buf[16];
        //ioctl(fileno(stdin), FIONREAD, &bytes);

        bytes = read(fd, buf, sizeof(buf));

        if (bytes <= 0) {
            return -1;
        }

        int parsed = parse_input(true,buf,bytes);

        key_hit = -1;
        key_released = -1;
        if ( !key_states[parsed % KEYBOARD_MAX] && (parsed / KEYBOARD_MAX) ) key_hit = parsed % KEYBOARD_MAX;
        if ( key_states[parsed % KEYBOARD_MAX] && !(parsed / KEYBOARD_MAX) ) key_released = parsed % KEYBOARD_MAX;
        if ( key_hit > 0 ) key_states[KEYBOARD_MAX + key_hit] = !key_states[KEYBOARD_MAX + key_hit];
        key_states[parsed % KEYBOARD_MAX] = !(parsed / KEYBOARD_MAX);

        return parsed;

        if (bytes == 0) {
            return -1;
        }
        

        char first_byte = getc(stdin);
        if (first_byte != 27) { return first_byte; }

        --bytes;
        if (bytes == 0) { return 27; }

        deque<char> esc_code;
        while (bytes > 0) {
            esc_code.push_back(getc(stdin));
            --bytes;
        }
        
        while (esc_code.size() > 4) {
			auto width = GetWindowWidth(), height = GetWindowHeight();
			if (esc_code[0] == '[' && esc_code[1] == '<' && esc_code[2] == '0' && esc_code[3] == ';') { //Mouse event
				// Handle mouse
			}
			else break;
		}
        if (esc_code.size() == 0) { return -1; }

        vector<char> code;
        string str = "";
        while (esc_code.size() > 0) {
            printf("%c",esc_code.front());
            if (esc_code.front() == ';') {
                code.push_back(stoi(str));
            } else {
                str.push_back(esc_code.front());
            }
            esc_code.pop_front();
        }
        if (str.back() != 'p') {
            str.pop_back();
        }
        int key_code = 0;
        for (auto i = 0; i < code.size(); i++) {
            key_code *= 0x100;
            key_code += code[i];
        }
        
        string print = '\n' + str + "\n" + to_string(code.size()) + "\n" + to_string(key_code) + "\n";
        
        return key_code;
    }

    void SysSleep(int microseconds){
        int ret;
        if (!microseconds % 1000000) ret = 1000000 * sleep(microseconds / 1000000);
        else ret = usleep(microseconds);
        if (ret) SysSleep(ret);
    }

    struct termios Console::old_termios = termios();
    struct termios Console::old_fdterm = termios();
    int Console::old_kbdmode = int();
    int Console::fd = int();
#endif

bool Console::initialised = false;
bitset<KEYBOARD_MAX*2> Console::key_states = bitset<KEYBOARD_MAX*2>(0);
int Console::key_hit = int();
int Console::key_released = int();

bool cpp::Console::KeyDown(int key) {
        return key_states[key];
    }

bool cpp::Console::KeyToggled(int key) {
    return key_states[KEYBOARD_MAX + key];
}

bool cpp::Console::KeyHit(int key) {
    return (cpp::Console::key_hit == key);
}

bool cpp::Console::KeyReleased(int key) {
    return (cpp::Console::key_released == key);
}

void cpp::Console::Sleep(double seconds) {

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