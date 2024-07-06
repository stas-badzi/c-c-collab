#include "main.hpp"

#include <iostream>
#include <dynamic_library.h>
#include <string>

libimport int* Console_FillScreen$ret2(void*** symbols, int height, int width);

libimport int Console_GetWindowWidth(void);

libimport int Console_GetWindowHeight(void);

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

using namespace cppimp;
using namespace std;

array<unsigned long,2> Console::FillConsole(vector<vector<void*>> symbols) {
    
    vector<void**> vpsym;
    for (size_t i = 0; i < symbols.size(); i++) {
        vpsym.push_back(symbols[i].data());
    }
    
    int* ret = Console_FillScreen$ret2(vpsym.data(),symbols.size(),symbols[0].size());
    // int* address dissapearing
    array<unsigned long,2> out;
    out[0] = ret[0];
    out[1] = ret[1];
    delete ret;
    return out;
}

int Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

int Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}

Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Constuct(L' ');
}

Console::Symbol::Symbol(wchar_t character, char foreground, char background) {
    symbol = Console_Symbol_Constuct(character,foreground,background);
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
    char Console::Symbol::GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void Console::Symbol::SetAttribute(char attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif

int main() {

    Console::Symbol sym(L'\u2588');

    //*
    

   
    while (true) {
        vector<vector<void*>> smbls;
        vector<void*> vsm;
        for (int i = 0; i < Console::GetWindowWidth(); i++)
        {
            vsm.push_back(sym.Get());
        }
        
        for (int i = 0; i < Console::GetWindowHeight(); i++)
        {
            smbls.push_back(vsm);
        }

        Console::FillConsole(smbls)[0];

        cout << Console::GetWindowWidth() << '\n';
        Sleep(1000);
    }
    
    cin.get();
    return 0;
}