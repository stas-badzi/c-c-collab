#include "class2.hpp"

#include <operating_system.h>
#include <iostream>

int class2::add(int a, int b) {
    return a + b;
}

void class2::hello() {
    std::cout << "Hello from C++ in " << OS_ << '!' << std::endl;
    return void();
}


libexport int class2_add(int a, int b) {
    return class2::add(a,b);
};

libexport  void class2_hello(void) {
    return class2::hello();
};
