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

    auto texture = FileSystem::TextureFromFile(L"C:\\Users\\micha\\Documents\\GitHub\\c-c-collab\\assets\\a.tux");
    cout << texture;
    FileSystem::FileFromTexture(L"C:\\Users\\micha\\Documents\\GitHub\\c-c-collab\\assets\\b.tux", texture);
    return 0;
}