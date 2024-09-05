#include "Console.hpp"

#include "dllimport.hpp"

#include <vector>

using namespace cppimp;
using namespace uniconv;
using namespace std;
using namespace cpp;

void Console::Init(void) {
    cppimp::Console_Init();
}

void Console::Fin(void) {
    cppimp::Console_Fin();
}

int Console::HandleKeyboard(void) {
    return cppimp::Console_HandleKeyboard();
}

bool Console::IsKeyDown(int key) {
    return cppimp::Console_IsKeyDown(key);
}

bool Console::IsKeyToggled(int key) {
    return cppimp::Console_IsKeyToggled(key);
}

int Console::KeyPressed(void) {
    return cppimp::Console_KeyPressed();
}

int Console::KeyReleased(void) {
    return cppimp::Console_KeyReleased();
}

array<unsigned long,2> Console::FillScreen(vector<vector<Console::Symbol> > symbols) {

    void*** voidsyms = new void**[symbols.size()];

    for (size_t i = 0; i < symbols.size(); i++) {
        void** temp = new void*[symbols[0].size()];
        for (size_t j = 0; j < symbols[0].size(); j++) {
            temp[j] = symbols[i][j].Get();
        }
        voidsyms[i] = temp;
    }

    long unsigned int* ret = Console_FillScreen$ret2(voidsyms,symbols.size(),symbols[0].size());
    
    for (size_t i = 0; i < symbols.size(); i++) {
        delete[] voidsyms[i];
    }
    delete[] voidsyms;

    array<unsigned long,2> out;
    out[0] = ret[0];
    out[1] = ret[1];
    delete[] ret;
    return out;
}

void Console::HandleMouseAndFocus(void) {
    return cppimp::Console_HandleMouseAndFocus();
}

bool Console::IsFocused(void) {
    return cppimp::Console_IsFocused();
}

struct ConsoleMouseStatus Console::GetMouseStatus(void) {
    return cppimp::Console_GetMouseStatus();
}

pair<uint8_t, uint8_t> Console::MouseButtonClicked(void) {
    uint8_t* func = cppimp::Console_MouseButtonClicked$ret2();
    pair<uint8_t, uint8_t> out(func[0],func[1]);
    delete[] func;
    return out;
}

uint8_t Console::MouseButtonReleased(void) {
    return cppimp::Console_MouseButtonReleased();
}

int16_t Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

int16_t Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}

void Console::Sleep(double seconds){
    Console_Sleep(seconds);
}

Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Construct$cfb(L' ');
}

Console::Symbol::Symbol(const Symbol &cp) {
    symbol = Console_Symbol_Construct$smb(cp.symbol);
}


Console::Symbol::Symbol(void* ptr, bool direct) {
    if (direct) symbol = ptr;
    else symbol = Console_Symbol_Construct$smb(ptr);
}

Console::Symbol::Symbol(utfchar character, uint8_t foreground, uint8_t background) {
    symbol = Console_Symbol_Construct$cfb(Utf8ToUnicode(character),foreground,background);
}

Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
}

Console::Symbol Console::Symbol::operator=(const Console::Symbol &src) {
    Console_Symbol_operator$eq(this->symbol,src.symbol);
    //set own local variables
    return *this;
}

utfchar Console::Symbol::character(void) {
    return UnicodeToUtf8(Console_Symbol_character$get(symbol));
}

void Console::Symbol::character(utfchar val) {
    return Console_Symbol_character$set(symbol, Utf8ToUnicode(val));
}

uint8_t Console::Symbol::foreground(void) {
    return Console_Symbol_foreground$get(symbol);
}

void Console::Symbol::foreground(uint8_t val) {
    return Console_Symbol_foreground$set(symbol, val);
}

uint8_t Console::Symbol::background(void) {
    return Console_Symbol_background$get(symbol);
}

void Console::Symbol::background(uint8_t val) {
    return Console_Symbol_background$set(symbol, val);
}

void* Console::Symbol::Get() {
    return symbol;
}

#ifdef _WIN32
    uint8_t Console::Symbol::GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void Console::Symbol::SetAttribute(uint8_t attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif
