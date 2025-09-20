#pragma once

#include <stdlib.h>

#include "utils/cextern.h"
#include "quick_exit/defines.h"

#define at_quick_exit _at_quick_exit
#define quick_exit _quick_exit

cextern void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void);
cextern int atquickexit__iterator;

inline int _at_quick_exit (void (*__func) (void)) {
    if (atquickexit__iterator >= _AT_QUICK_EXIT_MAX) return -1;
    atquickexit__funcs[atquickexit__iterator] = __func;
    ++atquickexit__iterator;
    return 0;
}

inline void _quick_exit( int exit_code ) {
    for (int i = 1; i <= atquickexit__iterator; i++)
        atquickexit__funcs[atquickexit__iterator - i]();
    _Exit(exit_code);
}