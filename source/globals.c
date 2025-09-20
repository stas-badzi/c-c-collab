#if (defined(_WIN32) && !defined(_MSC_VER)) || (defined(__APPLE__) && !defined(__cpp_lib_quick_exit))
#ifdef _WIN32
    void null(void) { return; }
#endif
    #include <quick_exit/defines.h>

    int atquickexit__iterator = 0;
    void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void) = { null };
#else
int __global = 1; // file cannot be empty
#endif

#ifdef _DEBUG
    void** ALLOCATIONS;
    unsigned long __size$ALLOCATIONS = 0;
    void** SYMBOLS;
    unsigned long __size$SYMBOLS = 0;
#endif
