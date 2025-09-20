#pragma once

#include <stdlib.h>

#include "utils/cextern.h"
#include "quick_exit/defines.h"

cextern void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void);
cextern int atquickexit__iterator;

inline int at_quick_exit (void (*__func) (void)) {
    if (atquickexit__iterator >= _AT_QUICK_EXIT_MAX) return -1;
    atquickexit__funcs[atquickexit__iterator] = __func;
    ++atquickexit__iterator;
    return 0;
}

inline void quick_exit( int exit_code ) {
    for (int i = 1; i <= atquickexit__iterator; i++)
        atquickexit__funcs[atquickexit__iterator - i]();
    _Exit(exit_code);
}