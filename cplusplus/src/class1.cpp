#include "class1.hpp"

libimport int class1_add(int a, int b, int c);
libimport void class1_hello(void);

int class1::add(int a, int b, int c) {
    return class1_add(a,b,c);
}

void class1::hello(void){
    return class1_hello();
}
