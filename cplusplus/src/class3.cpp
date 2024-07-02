#include "class3.hpp"

libimport int class3_add(int a, int b, int c, int d);
libimport void class3_hello(void);

int class3::add(int a, int b, int c, int d) {
    return class3_add(a,b,c,d);
}

void class3::hello(void){
    return class3_hello();
}
