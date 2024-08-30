#include "Console.hpp"
#include "FileSystem.hpp"

#include <unicode_conversion.hpp>

using namespace uniconv;
using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    Console::Init();
    vector<vector<Console::Symbol>> screen;
    screen = FileSystem::TextureFromFile("a.dat");
    while (1) { 
        Console::FillScreen(screen);
        Console::Sleep(0.05);
    }
    return 0;
}