#pragma once
#include <custom_defines.h>
#include <array>
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#endif

namespace cpp {
    class Console {

    public:
        struct Symbol {

            wchar_t character(void);
            void character(wchar_t val);

            byte foreground(void);
            void foreground(byte val);

            byte background(void);
            void background(byte val);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(wchar_t character, byte foreground = 7, byte background = 0);
            ~Symbol();

            Symbol operator=(const Symbol &src);

            void* Get();

            #ifdef _WIN32
                Symbol(byte attribute);
                byte GetAttribute(void);
                void SetAttribute(byte attribute);
            #endif

        private:
            void* symbol;

        };
        static void Init(void);
        static short int GetWindowWidth(void);
        static short int GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol>> symbols);
    };
}