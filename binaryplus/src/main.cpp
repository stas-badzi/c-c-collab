#include <iostream>
#include <windows.h>
#include <string>

#include "Console.hpp"
#include "FileSystem.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int main() {
    //Console::Init();
    auto texture = FileSystem::TextureFromFile(L"..\\assets\\a.tux");
    FileSystem::FileFromTexture(L"..\\assets\\b.tux", texture);
    return 0;
}