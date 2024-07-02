#include "main.hpp"

#include <iostream>
#include <dynamic_library.h>

libimport void class4_hello(void);

void class4::hello(void) {
    return class4_hello();
}

int main() {
    class4::hello();
    std::cout << "Hello from binary!";
    return 0;
}