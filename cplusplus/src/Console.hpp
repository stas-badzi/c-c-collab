#pragma once

#include <vector>
#include <array>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace cpp {
    class Console {
    private:
        static bool initialised;
        #ifdef _WIN32
            static HANDLE h_console;
        #endif
    public:
        static void Init(void);
        struct Symbol {
            wchar_t character;
            char foreground; // 0-16
            char background; // 0-16

            Symbol(void);

            Symbol(wchar_t character, char foreground = 7, char background = 0);

            Symbol(char attribute);

            ~Symbol(void);

            Symbol & operator=(const Symbol &src);

            #ifdef _WIN32
                char GetAttribute(void);
                void SetAttribute(char attribute);
            #endif

        };
        static short int GetWindowWidth(void);
        static short int GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol> > symbols);

    };
} // namespace cpp