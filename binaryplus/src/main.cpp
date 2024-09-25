#include <iostream>
#include <windows.h>
#include <string>

#include "Console.hpp"
#include "FileSystem.hpp"
#include "System.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int main() {
    //Console::Init();
    //auto texture = FileSystem::TextureFromFile(System::ToNativePath(System::GetRootPath() + L"/assets/a.tux"));
    //FileSystem::FileFromTexture(System::ToNativePath(System::GetRootPath() + L"/assets/b.tux"), texture);
    uint8_t fcolor[9] = {0,1,2,3,4,5,6,7,8};
    uint8_t bcolor[9] = {15,14,13,12,11,10,9,8,7};
    auto texture1 = Console::Symbol::CreateTexture(L"AAA\nBBB\nCCC\n", bcolor, fcolor);
    FileSystem::FileFromTexture(L"..\\assets\\c.tux", texture1);
    return 0;
}