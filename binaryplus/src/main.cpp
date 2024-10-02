#include <iostream>
#include <string>
#include <array>

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

        wstring* characters = new wstring[width]();
        uint8_t* back = new uint8_t[width]();
        uint8_t* fore = new uint8_t[width]();
        //po new jest delete;
        //ale jakby co masz vectory i vector.data() / vector.dat() nie pamietam
        for (int16_t i = 0; i < width; i++) {
            characters[i] = L'#';
            back[i] = 7;
            fore[i] = 2;
        }
        auto screen = Console::Symbol::CreateTexture(characters, height, back, fore);
        delete[] characters; delete[] back; delete[] fore;

        auto texture = FileSystem::TextureFromFile(System::ToNativePath(System::GetRootPath() + L"/assets/a.tux"));
        FileSystem::DrawTextureToScreen(3,10,texture,screen); //to nie zadziala bo screen jest za maly;
        Console::FillScreen(screen);
        Control::CleanMemory();
        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        auto mouse = Console::GetMouseStatus();
    }
    return 0;
}