#ifdef _WIN32
#ifndef _MSVC
    #include <quick_exit/defines.h>

    void null(void) { return; }

    int atquickexit__iterator = 0;
    void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void) = { null };
#endif // _MSVC
#endif // _WIN32