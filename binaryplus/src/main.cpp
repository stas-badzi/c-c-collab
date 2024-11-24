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

using namespace std;
using namespace cpp;
using namespace cs;

int main(void) {
    Console::Init();
    

    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    while (true){
    /*
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;
        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'#',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }

    */
        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        if ((Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRL_L) || Console::IsKeyDown(Key::Enum::CTRL_R)) && (Console::KeysToggled().CapsLock || Console::IsKeyDown(Key::Enum::SHIFT) || Console::IsKeyDown(Key::Enum::SHIFTL) || Console::IsKeyDown(Key::Enum::SHIFTR))) break;
        //if (Console::KeyPressed() != Key::Enum::NONE) cerr << (int)Console::KeyPressed() << endl;
    /*    
        auto mouse = Console::GetMouseStatus();

        //FileSystem::DrawTextureToScreen(20,2,pos,screen);

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

        */
    }
    return 0;
    
    /*}

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
                auto sym = Console::Symbol(L' ',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }
        //delete[] characters; delete[] back; delete[] fore;
        Console::HandleMouseAndFocus();
        
        if (Console::IsFocused()) Console::HandleKeyboard();
        auto mouse = Console::GetMouseStatus();
        //cerr << mouse.x << ' ' << mouse.y << '\n';
        //return 200;
        //screen[mouse.y][mouse.x].character(L'▦');

        //Console::FillScreen(screen);
        wstring a = System::GetRootPath();
        a.append(L"/assets/a.tux");
        wstring file = (argc < 2) ? System::GetRootPath() + L"/assets/a.tux" : wstring(argv[1]);
        auto texture = FileSystem::TextureFromFile(System::ToNativePath(file));
        Console::FillScreen(texture);
        Console::Sleep(10);
        Console::FillScreen(screen);
        Console::Sleep(10);
        FileSystem::DrawTextureToScreen(2,2,texture,screen); // już zaziała //to nie zadziala bo screen jest za maly;

        wstringstream wstr;
        wstr << mouse.x << L' ' << mouse.y << L"                            ";
        auto pos = Console::Symbol::CreateTexture(wstr.str());
        //FileSystem::DrawTextureToScreen(20,2,pos,screen);

        Console::FillScreen(pos);

        Control::CleanMemory();

        Console::Sleep(2.5);

        if (Console::KeyPressed() == Key::Enum::Q && (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRL_L) || Console::IsKeyDown(Key::Enum::CTRL_R))) return 1;
    }
    
    return 0;//*/
}