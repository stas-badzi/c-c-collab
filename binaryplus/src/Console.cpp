#include "Console.hpp"

#include "dllimport.hpp"

#include <vector>

using namespace cppimp;
using namespace std;

void cpp::Console::Init(void) {
    cppimp::Console_Init();
}

array<unsigned long,2> cpp::Console::FillScreen(vector<vector<cpp::Console::Symbol>> symbols) {

    void*** voidsyms = new void**[symbols.size()];

    for (size_t i = 0; i < symbols.size(); i++) {
        void** temp = new void*[symbols[0].size()];
        for (size_t j = 0; j < symbols[0].size(); j++) {
            temp[j] = symbols[i][j].Get();
        }
        voidsyms[i] = temp;
    }

    long unsigned int* ret = Console_FillScreen$ret2(voidsyms,symbols.size(),symbols[0].size());
    // int* address dissapearing
    array<unsigned long,2> out;
    out[0] = ret[0];
    out[1] = ret[1];
    delete ret;
    return out;
}

short int cpp::Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

short int cpp::Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}

cpp::Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Constuct$cfb(L' ');
}

cpp::Console::Symbol::Symbol(const Symbol &cp) {
    symbol = Console_Symbol_Constuct$smb(cp.symbol);
}

cpp::Console::Symbol::Symbol(wchar_t character, char foreground, char background) {
    symbol = Console_Symbol_Constuct$cfb(character,foreground,background);
}

cpp::Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
}

cpp::Console::Symbol cpp::Console::Symbol::operator=(const cpp::Console::Symbol &src) {
    Console_Symbol_operator$eq(this->symbol,src.symbol);
    //set own local variables
    return *this;
}

wchar_t cpp::Console::Symbol::character(void) {
    return Console_Symbol_character$get(symbol);
}

void cpp::Console::Symbol::character(wchar_t val) {
    return Console_Symbol_character$set(symbol, val);
}

char cpp::Console::Symbol::foreground(void) {
    return Console_Symbol_foreground$get(symbol);
}

void cpp::Console::Symbol::foreground(char val) {
    return Console_Symbol_foreground$set(symbol, val);
}

char cpp::Console::Symbol::background(void) {
    return Console_Symbol_background$get(symbol);
}

void cpp::Console::Symbol::background(char val) {
    return Console_Symbol_background$set(symbol, val);
}

void* cpp::Console::Symbol::Get() {
    return symbol;
}

#ifdef _WIN32
    char cpp::Console::Symbol::GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void cpp::Console::Symbol::SetAttribute(char attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif
