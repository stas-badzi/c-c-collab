#include "Console.hpp"

#include "dllimport.hpp"

#include <vector>

using namespace cppimp;
using namespace std;


array<unsigned long,2> cpp::Console::FillConsole(vector<vector<void*>> symbols) {
    
    vector<void**> vpsym;
    for (size_t i = 0; i < symbols.size(); i++) {
        vpsym.push_back(symbols[i].data());
    }
    
    long unsigned int* ret = Console_FillScreen$ret2(vpsym.data(),symbols.size(),symbols[0].size());
    // int* address dissapearing
    array<unsigned long,2> out;
    out[0] = ret[0];
    out[1] = ret[1];
    delete ret;
    return out;
}

int cpp::Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

int cpp::Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}

cpp::Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Constuct(L' ');
}

cpp::Console::Symbol::Symbol(wchar_t character, char foreground, char background) {
    symbol = Console_Symbol_Constuct(character,foreground,background);
}

cpp::Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
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
