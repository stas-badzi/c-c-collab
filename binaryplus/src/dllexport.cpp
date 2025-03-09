#include "dllimport.hpp"

using namespace cppimp;

extern int sub(int type);

extern "C" int Console_sub(int arg1) {
    return sub(arg1);
}

extern int Main(void);

int main(void) {
    Main$define(Main);
    return Main();
}