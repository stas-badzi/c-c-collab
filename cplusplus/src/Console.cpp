#include "Console.hpp"

using namespace cpp;
using namespace uniconv;
using namespace std;

#ifdef _WIN32

    inline uint8_t GenerateAtrVal(uint8_t i1, uint8_t i2) {
        uint8_t val;
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

    void Console::Init(void) {
        if (!initialised) {
            SetConsoleActiveScreenBuffer(Console::h_console);
            initialised = true;
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

		for (auto i = 0; i < win_height; i++) {
			for (auto j = 0; j < win_width; j++) {
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
        cout << val;
        return val;
    }

    void Console::Init(void) {
        if (!initialised) {
            initialised = true;
        }
    }

    int16_t cpp::Console::GetWindowWidth(void) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        return size.ws_col;
    }

    int16_t cpp::Console::GetWindowHeight(void) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        return size.ws_row;
    }

    array<unsigned long,2> Console::FillScreen(vector<vector<Console::Symbol> > symbols) {
        cout << "\e[H";
        size_t width = GetWindowWidth(), height = GetWindowHeight();
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                if (i >= symbols.size() || j >= symbols[0].size()) {
                    cout << "\033[0m ";
                    continue;
                }
                cout << GenerateEscapeSequence(symbols[i][j].foreground, symbols[i][j].background) << symbols[i][j].character;
            }
        }
        cout << "\033[0m";
        
        array<unsigned long,2> written;
				
		written[0] = width*height;
        written[1] = width*height + 1;

        return written;
    }
#endif

bool Console::initialised = false;

Console::Symbol::Symbol(utfchar character, uint8_t foreground, uint8_t background) {
    this->character = character;
    this->foreground = foreground;
    this->background = background;
}

Console::Symbol::Symbol(void) {
    this->character = L' ';
    this->foreground = 7;
    this->background = 0;
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