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
    //uint8_t fcolor[9] = {0,1,2,3,4,5,6,7,8};
    //uint8_t bcolor[9] = {15,14,13,12,11,10,9,8,7};
    //auto texture1 = FileSystem::TextureFromFile(System::GetRootPath() + System::ToNativePath(L"/assets/a.tux"));
    //FileSystem::FileFromTexture(System::GetRootPath() + System::ToNativePath(L"/assets/c.tux"), texture1);
    void* ptr = System::AllocateMemory(1);
    int a = System::ReadPointer<int>(ptr);
    cout << a;
    return 0;
}