#include <iostream>
#include <windows.h>
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
        auto screen = Console::Symbol::CreateTexture(System::MakeList(width, (wstring) "#"), height, System::MakeList(width, 7), System::MakeList(width, 7));
        auto texture = FileSystem::TextureFromFile(System::ToNativePath(System::GetRootPath() + L"/assets/a.tux"));
        FileSystem::DrawTextureToScreen(3,2,texture,screen);
        Console::FillScreen(screen);
        Control::CleanMemory();
    }
    return 0;
}