#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>

#include "Console.hpp"
#include "FileSystem.hpp"
#include "System.hpp"
#include "Control.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int wmain(int argc, wchar_t* argv[]) {
    Console::Init();
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
        auto file =(argc < 2) ? L"C:\\Users\\sssba\\Documents\\c-c-collab\\assets\\a.tux" : argv[1];
        auto texture = FileSystem::TextureFromFile(System::ToNativePath(wstring(file)));
        //Console::FillScreen(texture);
        //Console::FillScreen(screen);
        FileSystem::DrawTextureToScreen(2,2,texture,screen); // już zaziała //to nie zadziala bo screen jest za maly;

        wstringstream wstr;
        wstr << mouse.x << L' ' << mouse.y << L"                            ";
        auto pos = Console::Symbol::CreateTexture(wstr.str());
        FileSystem::DrawTextureToScreen(20,2,pos,screen);

        
        Console::FillScreen(screen);

        Control::CleanMemory();

        if (Console::KeyPressed() == 'Q' && Console::IsKeyDown(VK_CONTROL)) return 1;
    }
    return 0;
}