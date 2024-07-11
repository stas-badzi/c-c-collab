#pragma once

#include <custom_defines.h>
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
            byte foreground; // 0-16
            byte background; // 0-16

            Symbol(void);

            Symbol(wchar_t character, byte foreground = 7, byte background = 0);

            Symbol(byte attribute);

            ~Symbol(void);

            Symbol & operator=(const Symbol &src);

            #ifdef _WIN32
                byte GetAttribute(void);
                void SetAttribute(byte attribute);
            #endif

        };
        static short int GetWindowWidth(void);
        static short int GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol>> symbols);

    };
} // namespace cpp