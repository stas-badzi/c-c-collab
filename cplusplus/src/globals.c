#ifdef _WIN32
    #include <quick_exit/defines.h>

    void null(void) { return; }

    int atquickexit__iterator = 0;
    void (*atquickexit__funcs[_AT_QUICK_EXIT_MAX]) (void) = { null };
#endif // _WIN32