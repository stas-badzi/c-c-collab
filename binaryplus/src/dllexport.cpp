#include <dynamic_library.h>
#include "Console.hpp"

libexport int Console_sub(int arg1) {
    return sub(arg1);
}