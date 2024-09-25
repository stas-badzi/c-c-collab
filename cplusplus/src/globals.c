#include <dynamic_library.h>

#ifdef _WIN32
#ifndef _MSVC
    #include <quick_exit/defines.h>

    void null(void) { return; }

    int atquickexit__iterator = 0;
    void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void) = { null };
#endif // _MSVC
#endif // _WIN32

#ifdef _DEBUG
    void** ALLOCATIONS;
    unsigned long __size$ALLOCATIONS = 0;
    void** SYMBOLS;
    unsigned long __size$SYMBOLS = 0;
#endif