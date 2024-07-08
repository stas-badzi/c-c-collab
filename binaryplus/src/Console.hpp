#pragma once
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

            char foreground(void);
            void foreground(char val);

            char background(void);
            void background(char val);

            Symbol(void);
            Symbol(const Symbol &cp);
            Symbol(wchar_t character, char foreground = 7, char background = 0);
            ~Symbol();

            Symbol operator=(const Symbol &src);

            void* Get();

            #ifdef _WIN32
                Symbol(char attribute);
                char GetAttribute(void);
                void SetAttribute(char attribute);
            #endif

        private:
            void* symbol;

        };
        static void Init(void);
        static int GetWindowWidth(void);
        static int GetWindowHeight(void);
        static std::array<unsigned long,2> FillScreen(std::vector<std::vector<Symbol>> symbols);
    };
}