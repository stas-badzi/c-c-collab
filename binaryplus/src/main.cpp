#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>


using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    Console::Init();
    //cout << Console::GetWindowHeight() << '\n' << Console::GetWindowWidth();
    //return 0;
    //while (true) {
        Console::Symbol sym;
        sym.character(WCharToNative(L'▚'));
        vector<vector<Console::Symbol>> list;
        for (int l = 0; l < Console::GetWindowHeight(); l++) {
            vector<Console::Symbol> row;
            for (int i = 0; i < Console::GetWindowWidth(); i++) {
                row.push_back(sym);
                row.back().foreground(rand() % 16);
                row.back().background(rand() % 16);
            }
            list.push_back(row);
        }
        /*
        cout << list.size() << ' ' << list[0].size() << '\n';
        for (int i = 0; i < list.size(); i++)
        {
            for (int j = 0; j < list[0].size(); j++)
            {
             wcout << L'\x9626';
            }
        }*/
        
        
        Console::FillScreen(list);
        Sleep(10000);

    //}

    return 0;
}