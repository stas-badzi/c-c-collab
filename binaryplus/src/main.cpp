#include "main.hpp"

#include <iostream>
#include <dynamic_library.h>

libimport void class4_hello(void);
libimport int class4_add(int a, int b ,int c, int d, int e);

void class4::hello(void) {
    return class4_hello();
}

int main() {
    class4::hello();
    std::cout << "Hello from binary!\n" << class4_add(0,2,4,6,3);
    return 0;
}