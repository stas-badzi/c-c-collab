#include "Console.hpp"

using namespace std;
using namespace cpp;

#ifdef _WIN32

    #define set_atr_vals WORD atr_vals[16][16] = { { 0x0000 } }; for (auto i1 = 0; i1 < 16; i1++) { for (auto i2 = 0; i2 < 16; i2++) { WORD val = 0x0000; if (i1 == 0) { val |= 0x0000; } if (i1 == 1) { val |= FOREGROUND_RED; } if (i1 == 2) { val |= FOREGROUND_GREEN; } if (i1 == 3) { val |= FOREGROUND_RED | FOREGROUND_GREEN; } if (i1 == 4) { val |= FOREGROUND_BLUE; } if (i1 == 5) { val |= FOREGROUND_RED | FOREGROUND_BLUE; } if (i1 == 6) { val |= FOREGROUND_BLUE | FOREGROUND_GREEN; } if (i1 == 7) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; } if (i1 == 8) { val |= FOREGROUND_INTENSITY; } if (i1 == 9) { val |= FOREGROUND_RED | FOREGROUND_INTENSITY; } if (i1 == 10) { val |= FOREGROUND_GREEN | FOREGROUND_INTENSITY; } if (i1 == 11) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; } if (i1 == 12) { val |= FOREGROUND_BLUE | FOREGROUND_INTENSITY; } if (i1 == 13) { val |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; } if (i1 == 14) { val |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; } if (i1 == 15) { val |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; } if (i2 == 0) { val |= 0x0000; } if (i2 == 1) { val |= BACKGROUND_RED; } if (i2 == 2) { val |= BACKGROUND_GREEN; } if (i2 == 3) { val |= BACKGROUND_RED | BACKGROUND_GREEN; } if (i2 == 4) { val |= BACKGROUND_BLUE; } if (i2 == 5) { val |= BACKGROUND_RED | BACKGROUND_BLUE; } if (i2 == 6) { val |= BACKGROUND_BLUE | BACKGROUND_GREEN; } if (i2 == 7) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; } if (i2 == 8) { val |= BACKGROUND_INTENSITY; } if (i2 == 9) { val |= BACKGROUND_RED | BACKGROUND_INTENSITY; } if (i2 == 10) { val |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; } if (i2 == 11) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY; } if (i2 == 12) { val |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; } if (i2 == 13) { val |= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY; } if (i2 == 14) { val |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY; } if (i2 == 15) { val |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY; } atr_vals[i1][i2] = val; } } bool atr_vals_defined = 1

    HANDLE Console::h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    array<unsigned long,2> Console::FillConsole(vector<vector<Symbol>> symbols) {
        const size_t height = symbols.size();
        const size_t width = height > 0 ? symbols[0].size() : 0;

        wchar_t* screen = new wchar_t[width*height];
        WORD* attributes = new WORD[width*height];

		for (auto i = 0; i < height; i++) {
			for (auto j = 0; j < width; j++) {
				screen[ i*width + j ] = symbols[i][j].character;
                attributes[ i*width + j ] = symbols[i][j].GetAttribute();
			}
		}

        array<unsigned long,2> written;
				
		WriteConsoleOutputCharacter(h_console, screen, width*height, { 0,0 }, &(written[0]) );
		WriteConsoleOutputAttribute(h_console, attributes, width*height, { 0,0 }, &(written[1]) );
				
		delete screen;
		delete attributes;

        return written;
    }

    
    char Console::Symbol::GetAttribute(void) {
        set_atr_vals;
        return atr_vals[this->foreground][this->background];
    }

    void Console::Symbol::SetAttribute(char attribute) {
        this->foreground = 7;
        this->background = 0;
    }

    Console::Symbol::Symbol(char attribute) {
    this->character = L' ';
    this->SetAttribute(attribute);
}
#else
#include <iostream>
    array<unsigned long,2> Console::FillConsole(vector<vector<Symbol>> symbols) {
        for (size_t i = 0; i < symbols.size(); i++) {
            for (size_t j = 0; j < symbols[0].size(); j++) {
                wcout << symbols[i][j].character;
            }
            wcout << L'\n';
        }
        
        array<unsigned long,2> written;
				
		written[1] = 0;
        written[0] = symbols.size()*symbols[0].size();

        return written;
    }
#endif

Console::Symbol::Symbol(wchar_t character, char foreground, char background) {
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
    //delete this;
}