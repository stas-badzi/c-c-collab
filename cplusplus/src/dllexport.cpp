#include <dynamic_library.h>

// Console
    #include "Console.hpp"

    libexport void Console_Init(void) {
        cpp::Console::Init();
    }

    libexport unsigned long int* Console_FillScreen$ret2(cpp::Console::Symbol*** symbols, int height, int width) {
        std::vector<std::vector<cpp::Console::Symbol>> vecsyms;
        for (int i = 0; i < height; i++) {
            std::vector<cpp::Console::Symbol> vecdsym;
            for (int j = 0; j < width; j++) {
                vecdsym.push_back( *(symbols[i][j]) );
            }
            vecsyms.push_back(vecdsym);
        }
        unsigned long int* out = new unsigned long int[2];
        std::array<unsigned long int,2> ret = cpp::Console::FillScreen(vecsyms);
        out[0] = ret[0];
        out[1] = ret[1];
        return out;
    }

    libexport short int Console_GetWindowWidth(void) {
        return cpp::Console::GetWindowWidth();
    }

    libexport short int Console_GetWindowHeight(void) {
        return cpp::Console::GetWindowHeight();
    }

    // Symbol
        libexport void* Console_Symbol_Constuct$smb(cpp::Console::Symbol* src) {
            return (void*) new cpp::Console::Symbol(*src);
        }


        libexport void* Console_Symbol_Constuct$cfb(wchar_t character, char foreground = 7, char background = 0) {
            return (void*) new cpp::Console::Symbol(character, foreground, background);
        }

        #ifdef _WIN32
            libexport void* Console_Symbol_Constuct$atr(char attribute) {
                return (void*) new cpp::Console::Symbol(attribute);
            }
            
            libexport void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, char attribute) {
                smb->SetAttribute(attribute);
            }

            libexport char Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
                return smb->GetAttribute();
            }
        #endif

        libexport void Console_Symbol_character$set(cpp::Console::Symbol* smb, wchar_t character) {
            smb->character = character;
        }

        libexport char Console_Symbol_character$get(cpp::Console::Symbol* smb) {
            return smb->character;
        }

        libexport void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, wchar_t foreground) {
            smb->foreground = foreground;
        }

        libexport char Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
            return smb->foreground;
        }

        libexport void Console_Symbol_background$set(cpp::Console::Symbol* smb, wchar_t background) {
            smb->background = background;
        }

        libexport char Console_Symbol_background$get(cpp::Console::Symbol* smb) {
            return smb->background;
        }

        libexport void Console_Symbol_Destruct(cpp::Console::Symbol* smb) {
            delete smb;
        }

        libexport void* Console_Symbol_operator$eq(cpp::Console::Symbol* cp, cpp::Console::Symbol* src) {
            return (void*) new cpp::Console::Symbol( (*cp) = (*src) );
        }
    // ~Symbol

// ~Console