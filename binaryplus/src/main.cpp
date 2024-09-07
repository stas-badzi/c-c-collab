#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    int mode1,mode2,fd,err;

    Console::Init();
    vector<vector<Console::Symbol>> screen;
    while (1) {
        screen = FileSystem::TextureFromFile(WStringToNative(L"a.tux"));
        Console::HandleMouseAndFocus();
        struct ConsoleMouseStatus mouse = Console::GetMouseStatus();
        wstring x = to_wstring(mouse.x);
        wstring y = to_wstring(mouse.y);
        if (mouse.x < UINT32_MAX) {
            for (int i = 0; i < x.size(); i++) {
                screen[0][i].character(WCharToNative(x[i]));
            }
        }
        Console::FillScreen(screen);
        Console::Sleep(0.05);
    }
    return 0;
}