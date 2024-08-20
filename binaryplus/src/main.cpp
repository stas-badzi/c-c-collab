#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <thread>

#include <unistd.h>


using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    Console::Init();

    while (true)
    {
        thread kbd_handle = thread([&] {
            while (!(Console::KeyDown(29) && Console::KeyHit(16))) {
                int out = Console::HandleKeyboard();
                const char * action = (out / 256) ? " up" : " down";
                if (out > 0) if (!Console::KeyDown(56)) cerr << out % 256 << action << '\n';
            }
        });
        while (1) {
            if (Console::KeyDown(125)) cerr << '^' << ':' << Console::KeyDown(29) << ' ' << 'q' << ':' << Console::KeyDown(16) << '\n';
            if (Console::KeyDown(29) && Console::KeyHit(16)) {
                cerr << "Exiting";
                usleep(300000);
                cerr << '.';
                sleep(1);
                cerr << '.';
                sleep(1);
                cerr << '.';
                usleep(500000);
                exit(0);
            }
        }
        
        
    }
    
/*
    while (true) {
        
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
        
        Console::FillScreen(list);

    }
*/
    return 0;
}