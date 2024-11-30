#include "Console.hpp"
#include "FileSystem.hpp"
#include "System.hpp"
#include "Control.hpp"

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <chrono>

#ifndef VK_CONTROL
    #define VK_CONTROL 10
#endif

#define IsCtrlDown() (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRLL) || Console::IsKeyDown(Key::Enum::CTRLR))

using namespace std;
using namespace cpp;
using namespace cs;

int main(void) {
    Console::Init();
    

    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    //auto last_mouse = Console::GetMouseStatus();
    //while (true){
    /*
        uint16_t width = Console::GetWindowWidth();
        uint16_t height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;
        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'▒',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }
    //*/
        //Console::HandleMouseAndFocus();
        //if (Console::IsFocused()) Console::HandleKeyboard();
        //if (Console::KeyPressed() != Key::Enum::NONE) cerr << (int)Console::KeyPressed() << '\n';
        //if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return 0;
        /*
        auto mouse = Console::GetMouseStatus();
        if (0 <= mouse.x && mouse.x < width && 0 <= mouse.y && mouse.y < height) {
            screen[mouse.y][mouse.x].character(L'🖱');
            if (mouse.x > 0) screen[mouse.y][mouse.x-1].character(L'▋');
            if (mouse.x < width-1) { screen[mouse.y][mouse.x+1].character(L'▐'); screen[mouse.y][mouse.x+1].foreground(9); screen[mouse.y][mouse.x+1].background(3); }
        }
        //if (mouse.x != last_mouse.x || mouse.y != last_mouse.y || mouse.primary != last_mouse.primary || mouse.secondary != last_mouse.secondary || mouse.middle != last_mouse.middle || mouse.scroll.first != last_mouse.scroll.first || mouse.scroll.second != last_mouse.scroll.second)
        //    cerr << mouse.x << ' ' << mouse.y << '\n' << mouse.primary << ' ' << mouse.secondary << ' ' << mouse.middle << ' ' << mouse.scroll.first << ' ' << mouse.scroll.second << '\n';
        //last_mouse = mouse;
        //vector<vector<Console::Symbol>> sym = {1,{1,{L'▦',16,16}}}; 
        Console::FillScreen(screen);
        Console::Sleep(0.1);

        //FileSystem::DrawTextureToScreen(20,2,pos,screen);
        */
        /**//* 
        if (max(mouse.y-1,0u) >= 0 && max(mouse.x-1,0u) >= 0 && max(mouse.y-1,0u) < height && max(mouse.x-1,0u) < width)
            screen[max(mouse.y-1,0u)][max(mouse.x-1,0u)].background(5);
        else
            screen[0][0].foreground(5);

        Console::FillScreen(screen);

        //Control::CleanMemory();

        auto enlapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
        wstringstream wstr;
        wstr << "[  " << enlapsed_time << "MPF  ]";
        auto pos = Console::Symbol::CreateTexture(wstr.str());
        Console::Sleep(1);
        Console::FillScreen(pos);
        Console::Sleep(1);
        start = chrono::high_resolution_clock::now();

        //*/
    //}
    //return 0;
    
    //*}

    int argc = Console::GetArgC();
    wchar_t** argv = Console::GetArgV();
    while (true) {
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;

        //wstring* characters = new wstring[(width+1)*height - 1]();
        //uint8_t* back = new uint8_t[width]();
        //uint8_t* fore = new uint8_t[width]();
        //po new jest delete;
        //ale jakby co masz vectory i vector.data() / vector.dat() nie pamietam
        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'▒',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }
        //delete[] characters; delete[] back; delete[] fore;
        //cerr << mouse.x << ' ' << mouse.y << '\n';
        //return 200;
        //screen[mouse.y][mouse.x].character(L'▦');

        //Console::FillScreen(screen);
        wstring a = System::GetRootPath();
        a.append(L"/assets/a.tux");
        wstring file = (argc < 2) ? System::GetRootPath() + L"/assets/a.tux" : wstring(argv[1]);
        auto texture = FileSystem::TextureFromFile(System::ToNativePath(file));
        FileSystem::DrawTextureToScreen(2,2,texture,screen); // już zaziała //to nie zadziala bo screen jest za maly;

        auto menu = Console::Symbol::CreateTexture(L"| Quit | Save | Load | Edit | Help |", array<uint8_t,70>{'\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7'}.data(), array<uint8_t,70>{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}.data());

        Console::HandleMouseAndFocus();
        auto mouse = Console::GetMouseStatus();
        if (mouse.x > 0 && mouse.y == 0 && mouse.x < 7) {
            for (int i = 1; i < 7; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                Control::CleanMemory();
                Console::Fin();
                return EXIT_SUCCESS;
            }
        }

        FileSystem::DrawTextureToScreen(0,0,menu,screen);

        Console::FillScreen(screen);

        Control::CleanMemory();

        if (Console::IsFocused()) Console::HandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return EXIT_SUCCESS;
        //FileSystem::DrawTextureToScreen(20,2,pos,screen);

    }
    return EXIT_FAILURE;
}