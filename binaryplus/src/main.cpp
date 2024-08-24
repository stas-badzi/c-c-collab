#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <thread>

#ifdef _WIN32
    #define at_quick_exit(func) atexit(func)
    #define quick_exit(code) exit(code)
    #define console_clear "cls"
#else
    #define console_clear "clear"
#endif


using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    Console::Init();
    vector<vector<Console::Symbol>> scr;
    
//*
    while (true) {
        
        /*
        thread display([&] {
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
        });
        //*/
        cerr << "a\n";
            int out = Console::HandleKeyboard();
            cerr << out << '\n';
            const char * action = (out / 256) ? " up" : " down";

            if (Console::KeyDown(125)) cerr << '^' << ':' << Console::KeyDown(29) << ' ' << 'q' << ':' << Console::KeyDown(16) << '\n';
            if (Console::KeyDown(29) && Console::KeyHit(16)) {
                int out = system(console_clear);
                cerr << "Exiting";
                cerr << '.';
                cerr << '.';
                cerr << '.';
                exit(0);
            }
            if (out > 0) if (!Console::KeyDown(56)) cerr << out % 256 << action << '\n';
        
            int action_size = (out / 256) ? 3 : 5;

        thread kbd_handle = thread([&] {
            
            /*if (scr.size() >= Console::GetWindowHeight()) {
                scr.erase(scr.begin());
            }
            scr.push_back(vector<Console::Symbol>());
            int out_size = to_string(out % 256).size();
            if (out > 0) if (!Console::KeyDown(56)) for (int i = 0; i < out_size + action_size; i++) {
                if (i < out_size) {
                    scr[0].push_back(uniconv::WCharToNative(to_wstring(out % 256)[i]));
                } else if (i - out_size < action_size) {
                    scr[0].push_back(uniconv::WCharToNative(action[i - out_size]));
                }
                
            }
            Console::FillScreen(scr);*/
            
        });
        
        
        //display.join();
        kbd_handle.join();

        Console::Sleep(1.0);
    }
//*/
    return 0;
}