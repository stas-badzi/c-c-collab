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

    inline COLORREF GetPixel(int x, int y, int width, int height, RGBQUAD* pixels) {
        return RGB(pixels[(height-y-1)*width + x].rgbRed, pixels[(height-y-1)*width + x].rgbGreen, pixels[(height-y-1)*width + x].rgbBlue);
    }
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
*/
/*
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
    HANDLE Console::screen = HANDLE();
    HANDLE Console::fd = HANDLE();
    HWND Console::window = HWND();
    HDC Console::device = HDC();
    DWORD Console::old_console = DWORD();
    uint8_t Console::default_fcol = uint8_t();
    uint8_t Console::default_bcol = uint8_t();
    utfcstr* Console::argv = (utfcstr*)malloc(0);
    //pair<uint16_t,uint16_t> Console::xyoffset = pair<uint16_t,uint16_t>();

    inline HWND GetHwnd(void) {
        HWND hwndFound;
        wchar_t pszNewWindowTitle[1024];
        wsprintf(pszNewWindowTitle,L"%d/%d",GetTickCount(),GetCurrentProcessId());
        SetConsoleTitle(pszNewWindowTitle);
        SysSleep(40e3);
        hwndFound=FindWindow(NULL, pszNewWindowTitle);
        return(hwndFound);
    }

    void Console::Init(void) {
        if (!initialised) {
            Console::screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            SetConsoleActiveScreenBuffer(Console::screen);
            
            Console::fd = GetStdHandle(STD_INPUT_HANDLE);
            GetConsoleMode(Console::fd, &Console::old_console);
            SetConsoleMode(Console::fd, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

            Console::window = GetHwnd();
            Console::device = GetDC(Console::window);
            //Console::xyoffset = Console::GetXYCharOffset();

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(Console::screen, &csbi);
            auto val = AtrValToColors(csbi.wAttributes);
            Console::default_fcol = val.first;
            Console::default_bcol = val.second;
            
            Console::mouse_status.x = 0;
            Console::mouse_status.y = 0;
            
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
            SetConsoleMode(Console::fd, old_console);
            SetConsoleActiveScreenBuffer((HANDLE)nullptr);

            initialised = false;
        }
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

    void Console::FillScreen(vector<vector<Symbol> > symbols) {
        Symbol empty_sym = Symbol(L' ', 16, 16);
        const size_t win_width = GetWindowWidth(), win_height = GetWindowHeight(), height = symbols.size(), width = height > 0 ? symbols[0].size() : 0;

        wchar_t* screen = new wchar_t[win_height*win_width];
        WORD* attributes = new WORD[win_height*win_width];

		for (size_t i = 0; i < win_height; i++) {
			for (size_t j = 0; j < win_width; j++) {
                if (i >= height || j >= width) {
                    screen[ i*win_width + j ] =  empty_sym.character;
                    attributes[ i*win_width + j ] = empty_sym.GetAttribute();
                    continue;
                }
                screen[ i*win_width + j ] = symbols[i][j].character;
                attributes[ i*win_width + j ] = symbols[i][j].GetAttribute();
			}
		}

        array<DWORD,2> written;
		BOOL out = WriteConsoleOutputCharacter(Console::screen, screen, win_width*win_height, { 0,0 }, &(written[0]) );
        if (out == 0) { exit(GetLastError()); }
		out = WriteConsoleOutputAttribute(Console::screen, attributes, win_width*win_height, { 0,0 }, &(written[1]) );
        if (out == 0) { exit(GetLastError()); }
        
		delete[] screen;
		delete[] attributes;
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

        INPUT_RECORD record;
        DWORD evnts,numRead;
        
        if(!GetNumberOfConsoleInputEvents(Console::fd, &evnts)) {
            cerr << "GetNumberOfConsoleInputEvents";
            exit(GetLastError());
        }
        if (!evnts) return;
        for (unsigned i = 0; i < evnts; ++i) {
        
            if(!ReadConsoleInput(Console::fd, &record, 1, &numRead)) {
                cerr << "ReadConsoleInput";
                exit(GetLastError());
            }
            bitset<5> event(record.EventType);
            if (event[0]) {
                // KEY_EVENT
            }

            Console::this_mouse_button = -1;
            Console::this_mouse_down = false;
            Console::this_mouse_combo = 0;
            Console::mouse_buttons_down[3] = false;
            Console::mouse_buttons_down[4] = false;
            Console::mouse_status.scroll = {false,false};
            if (event[1]) {
                // MOUSE_EVENT

                auto mouse = record.Event.MouseEvent;
                bitset<4> flags(mouse.dwEventFlags);

                if (flags[0]) {
                    Console::mouse_status.x = mouse.dwMousePosition.X;
                    Console::mouse_status.y = mouse.dwMousePosition.Y;
                }
                Console::mouse_status.primary = GetKeyState(VK_LBUTTON) & 0x8000;
                Console::mouse_status.secondary = GetKeyState(VK_RBUTTON) & 0x8000;
                Console::mouse_status.middle = GetKeyState(VK_MBUTTON) & 0x8000;
                Console::mouse_status.scroll = { flags[3], flags[3] && HIWORD(mouse.dwButtonState) > 0 };
                Console::mouse_buttons_down[3] = flags[3] && HIWORD(mouse.dwButtonState) > 0;
                Console::mouse_buttons_down[4] = flags[3] && HIWORD(mouse.dwButtonState) > 0;
                Console::this_mouse_combo = flags[2] ? 2 : 1;
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

            FILE *cmdline = fopen("/proc/self/cmdline", "rb");
            char *arg = 0; size_t size = 0;
            int out = getdelim(&arg, &size, 0, cmdline);
            while(out != -1) {
                Console::argv = (const char**)realloc(Console::argv,++Console::argc); Console::argv[Console::argc-1] = arg;
                arg = 0; size = 0;
                out = getdelim(&arg, &size, 0, cmdline);
            }
            //if (numeric_limits<char>::is_signed) for (int i=0; i < Console::argc; ++i) for(int j=0; j < strlen(Console::argv[i]); ++j) ((char**)(Console::argv))[i][j] = (Console::argv)[i][j] + INT8_MIN;
            fclose(cmdline);

            fd = getfd(0);

            if (fd < 0) {
                string command = "sudo";
                for (int i = 0; i < Console::argc; i++) { command.push_back(' '); command.append(argv[i]); }
                exit(system(command.c_str()));
                
                string error = GenerateEscapeSequence(1,16) + "\nCouldn't get a file descriptor referring to the console.\nCheck if you have acces to /dev/tty and /dev/console.\n" + GenerateEscapeSequence(4,16) + "\n\ttry: " + GenerateEscapeSequence(6,16) + command.c_str() + "\033[0m\n";
                throw(runtime_error(error.c_str()));
            }

            char path[256];
            readlink( "/proc/self/exe" , path, 256);
            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode.bin ");
            command.push_back('0' + K_MEDIUMRAW);

            if (ioctl(fd, KDGKBMODE, &old_kbdmode)) {
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
            readlink( "/proc/self/exe" , path, 256);
            string command = path;
            while (command.back() != '/') command.pop_back();
            command.append("../share/factoryrush/bin/setkbdmode.bin ");
            command.push_back('0' + old_kbdmode);
            old_kbdmode = system(command.c_str());
            if (old_kbdmode < 0) {
                throw("setkbdmode.bin error");
            }

            for (int i = 0; i < argc; i++) free((void*)Console::argv[i]);
            Console::argv = (const char**)realloc(Console::argv,0);
            Console::argc = 0;
            

            initialised = false;
        }
    }

    void Console::HandleMouseAndFocus(void) {
        Console::this_mouse_button = -1;
        Console::this_mouse_down = false;
        Console::this_mouse_combo = 0;
        Console::mouse_buttons_down[3] = false;
        Console::mouse_buttons_down[4] = false;

        bool mousedown = false;
        int bytes;

        ioctl(fileno(stdin), FIONREAD, &bytes);

        if (bytes < 3) {
            return;
        }

        if (getc(stdin) != '\033') return;
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
            fullbutton |= 0b001000 * event[2]; // Shift
            fullbutton |= 0b010000 * event[3]; // Meta
            fullbutton |= 0b100000 * event[4]; // Ctrl
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
            
            switch (button) {
            case 0:
                Console::mouse_status.primary = mousedown;
                break;
            case 1:
                Console::mouse_status.middle = mousedown;
                break;
            case 2:
                Console::mouse_status.secondary = mousedown;
                break;
            case 4:
                Console::mouse_status.scroll = {mousedown,true};
                break;
            case 5:
                Console::mouse_status.scroll = {mousedown,false};
                break;
            }
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
    utfcstr* Console::argv = (const char**)malloc(0);

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


    int16_t Console::GetWindowWidth(void) {
        ioctl(STDERR_FILENO, TIOCGWINSZ, &Console::window_size);
        return Console::window_size.ws_col;
    }

    int16_t Console::GetWindowHeight(void) {
        ioctl(STDERR_FILENO, TIOCGWINSZ, &Console::window_size);
        return Console::window_size.ws_row;
    }

    void Console::FillScreen(vector<vector<Console::Symbol> > symbols) {
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
    }

    void SysSleep(int microseconds){
        int ret;
        if (!microseconds % 1000000) ret = 1000000 * sleep(microseconds / 1000000);
        else ret = usleep(microseconds);
        if (ret) SysSleep(ret);
    }

    struct termios Console::old_termios = termios();
    winsize Console::window_size = winsize();
    int Console::fd = int();
#endif

int Console::argc = 0;

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
    return !Console::this_mouse_down ? pair<uint8_t, uint8_t>(-1,0) : pair<uint8_t, uint8_t>(Console::this_mouse_button, Console::this_mouse_combo);
}

uint8_t Console::MouseButtonReleased(void) {
    return !Console::this_mouse_down ? Console::this_mouse_button : -1;
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