#if (defined(_WIN32) && !defined(_MSC_VER)) || defined(__APPLE__)
#ifdef __APPLE__
#include <stdlib.h>
__if_not_exists(quick_exit) {
#else
    void null(void) { return; }
#endif
    #include <quick_exit/defines.h>

    int atquickexit__iterator = 0;
    void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void) = { null };

#ifdef __APPLE__
}
#endif
#else
int __global = 1; // file cannot be empty
#endif

#ifdef _DEBUG
    void** ALLOCATIONS;
    unsigned long __size$ALLOCATIONS = 0;
    void** SYMBOLS;
    unsigned long __size$SYMBOLS = 0;
#endif
