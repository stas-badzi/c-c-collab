#include <iostream>
#include <string>
#include <array>
#include <vector>

#include "Console.hpp"
#include "FileSystem.hpp"
#include "System.hpp"
#include "Control.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int main() {
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
                auto sym = Console::Symbol(L'#',(uint8_t)7,(uint8_t)2);
                screen.back().push_back(sym);
            }
        }
        //delete[] characters; delete[] back; delete[] fore;

        auto texture = FileSystem::TextureFromFile(System::ToNativePath(System::GetRootPath() + L"/assets/a.tux"));
        FileSystem::DrawTextureToScreen(3,10,texture,screen); // już zaziała //to nie zadziala bo screen jest za maly;
        Console::FillScreen(screen);
        Control::CleanMemory();
        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        auto mouse = Console::GetMouseStatus();
    }
    return 0;
}