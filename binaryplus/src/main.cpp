#include <iostream>

#include "Console.hpp"
#include "FileSystem.hpp"

using namespace std;
using namespace cpp;
using namespace cs;

int main() {
    Console::Init();
    cerr << "Hello, World!\n";
    return 0;
}