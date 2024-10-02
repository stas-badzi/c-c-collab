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

        wstring characters[] = new wstring[width]();
        uint8_t colors[] = new uint8_t[width]();
        for (int16_t i = 0; i < width; i++) {
            characters[i] = "#";
            colors[i] = 7;
        }
        auto screen = Console::Symbol::CreateTexture(characters, height, colors, colors);
        auto texture = FileSystem::TextureFromFile(System::ToNativePath(System::GetRootPath() + L"/assets/a.tux"));
        FileSystem::DrawTextureToScreen(3,10,texture,screen);
        Console::FillScreen(screen);
        Control::CleanMemory();
        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        auto mouse = Console::GetMouseStatus();
    }
    return 0;
}