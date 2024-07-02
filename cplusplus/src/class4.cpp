#include "class4.hpp"

#include "class2.hpp"
#include "class3.hpp"

#include <iostream>

int class4::add(int a, int b, int c, int d, int e) {
    return class2::add(a,class3::add(b,c,d,e));
}

void class4::hello() {
    class3::hello();
    std::cout << "Hello again from C++!" << std::endl;
    return;
}

libexport int class4_add(int a, int b, int c, int d, int e) {
    return class4::add(a,b,c,d,e);
}

libexport void class4_hello(void) {
    return class4::hello();
}
