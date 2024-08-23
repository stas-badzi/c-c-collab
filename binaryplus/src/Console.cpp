#include "Console.hpp"

#include "dllimport.hpp"

#include <vector>

using namespace cppimp;
using namespace uniconv;
using namespace std;

void cpp::Console::Init(void) {
    cppimp::Console_Init();
}

void cpp::Console::Fin(void) {
    cppimp::Console_Fin();
}

int cpp::Console::HandleKeyboard(void) {
    return cppimp::Console_HandleKeyboard();
}

bool cpp::Console::KeyDown(int key) {
    return cppimp::Console_KeyDown(key);
}

bool cpp::Console::KeyToggled(int key) {
    return cppimp::Console_KeyToggled(key);
}

bool cpp::Console::KeyHit(int key) {
    return cppimp::Console_KeyHit(key);
}

bool cpp::Console::KeyReleased(int key) {
    return cppimp::Console_KeyReleased(key);
}

array<unsigned long,2> cpp::Console::FillScreen(vector<vector<cpp::Console::Symbol> > symbols) {

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

int16_t cpp::Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

int16_t cpp::Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}

void cpp::Console::Sleep(double seconds){
    Console_Sleep(seconds);
}

cpp::Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Construct$cfb(L' ');
}

cpp::Console::Symbol::Symbol(const Symbol &cp) {
    symbol = Console_Symbol_Construct$smb(cp.symbol);
}

cpp::Console::Symbol::Symbol(utfchar character, uint8_t foreground, uint8_t background) {
    symbol = Console_Symbol_Construct$cfb(Utf8ToUnicode(character),foreground,background);
}

cpp::Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
}

cpp::Console::Symbol cpp::Console::Symbol::operator=(const cpp::Console::Symbol &src) {
    Console_Symbol_operator$eq(this->symbol,src.symbol);
    //set own local variables
    return *this;
}

utfchar cpp::Console::Symbol::character(void) {
    return UnicodeToUtf8(Console_Symbol_character$get(symbol));
}

void cpp::Console::Symbol::character(utfchar val) {
    return Console_Symbol_character$set(symbol, Utf8ToUnicode(val));
}

uint8_t cpp::Console::Symbol::foreground(void) {
    return Console_Symbol_foreground$get(symbol);
}

void cpp::Console::Symbol::foreground(uint8_t val) {
    return Console_Symbol_foreground$set(symbol, val);
}

uint8_t cpp::Console::Symbol::background(void) {
    return Console_Symbol_background$get(symbol);
}

void cpp::Console::Symbol::background(uint8_t val) {
    return Console_Symbol_background$set(symbol, val);
}

void* cpp::Console::Symbol::Get() {
    return symbol;
}

#ifdef _WIN32
    uint8_t cpp::Console::Symbol::GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void cpp::Console::Symbol::SetAttribute(uint8_t attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif
