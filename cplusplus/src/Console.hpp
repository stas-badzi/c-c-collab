#pragma once

#include <vector>
#include <array>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace cpp {
    class Console {
    private:
        #ifdef _WIN32
            static HANDLE h_console;
        #endif
    public:
        struct Symbol {
            wchar_t character;
            char foreground; // 0-16
            char background; // 0-16

            Symbol(wchar_t character = L' ', char foreground = 7, char background = 0);

            Symbol(char attribute);

            ~Symbol(void);

            #ifdef _WIN32
                char GetAttribute(void);
                void SetAttribute(char attribute);
            #endif

        };
        static std::array<unsigned long,2> FillConsole(std::vector<std::vector<Symbol>> symbols);
    };
} // namespace cpp

