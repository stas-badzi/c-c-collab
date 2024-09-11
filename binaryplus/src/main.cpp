#include <iostream>
#include <windows.h>
#include <string>

#include "Console.hpp"
#include "FileSystem.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int main() {
    Console::Init();
    while (!Console::IsKeyDown(VK_CONTROL) || Console::KeyPressed() != 'Q') {
        Console::HandleMouseAndFocus();
        wstring val;
        val.append(L"Hello, World!" + to_wstring(Console::GetMouseStatus().y) + L" y: " + to_wstring(Console::GetMouseStatus().x) + L" abcdefg  h     ezzzzzzzzzzzzzzzzz");
        auto p = Console::Symbol::CreateTexture(val);
        Console::FillScreen(p);
        Console::HandleKeyboard();
    }
    return 0;
}