#include "main.hpp"

#include <iostream>
#include <dynamic_library.h>
#include <string>

libimport int* Console_FillConsole$ret2(void*** symbols, int height, int width);

libimport void* Console_Symbol_Constuct(wchar_t character, char foreground = 7, char background = 0);

#ifdef _WIN32
    libimport void* Console_Symbol_Constuct$atr(char attribute);
    
    libimport void Console_Symbol_SetAttribute(void* smb, char attribute);

    libimport char Console_Symbol_GetAttribute(void* smb);
#endif

libimport void Console_Symbol_character$set(void* smb, wchar_t character);

libimport char Console_Symbol_character$get(void* smb);

libimport void Console_Symbol_foreground$set(void* smb, wchar_t foreground);

libimport char Console_Symbol_foreground$get(void* smb);

libimport void Console_Symbol_background$set(void* smb, wchar_t background);

libimport char Console_Symbol_background$get(void* smb);

libimport void Console_Symbol_Destruct(void* smb);

using namespace cppi;
using namespace std;

array<unsigned long,2> Console::FillConsole(vector<vector<void*>> symbols) {
    
    vector<void**> vpsym;
    for (size_t i = 0; i < symbols.size(); i++) {
        vpsym.push_back(symbols[i].data());
    }
    
    int* ret = Console_FillConsole$ret2(vpsym.data(),symbols.size(),symbols[0].size());
    // int* address dissapearing
    array<unsigned long,2> out;
    out[0] = ret[0];
    out[1] = ret[1];
    delete ret;
    return out;
}

Console::Symbol::Symbol() {
    symbol = Console_Symbol_Constuct(L'A');
}

Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
}

wchar_t Console::Symbol::character(void) {
    return Console_Symbol_character$get(symbol);
}

void Console::Symbol::character(wchar_t val) {
    return Console_Symbol_character$set(symbol, val);
}

char Console::Symbol::foreground(void) {
    return Console_Symbol_foreground$get(symbol);
}

void Console::Symbol::foreground(char val) {
    return Console_Symbol_foreground$set(symbol, val);
}

char Console::Symbol::background(void) {
    return Console_Symbol_background$get(symbol);
}

void Console::Symbol::background(char val) {
    return Console_Symbol_background$set(symbol, val);
}

void* Console::Symbol::Get() {
    return symbol;
}

#ifdef _WIN32
    char GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void SetAttribute(char attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif

int main() {

    Console::Symbol sym;

    //*
    vector<vector<void*>> smbls;
    vector<void*> vsm;

    Console::Symbol symb;
    vsm.push_back(symb.Get());
    vsm.push_back(symb.Get());

    smbls.push_back(vsm);
    smbls.push_back(vsm);
    smbls.push_back(vsm);

    Console::FillConsole(smbls);
    //*/
    //cin.get();
    return 0;
}