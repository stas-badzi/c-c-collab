#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>


using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    Console::Init();

    //while (true) {
        Console::Symbol sym;
        sym.character(WCharToNative(L'▒'));
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
        
        Console::FillScreen(list);
    //}

    return 0;
}