#include <dynamic_library.h>

// Console
    #include "Console.hpp"

    libexport int* Console_FillConsole$ret2(cpp::Console::Symbol** symbols, int width, int height) {
        std::vector<std::vector<cpp::Console::Symbol>> vecsyms;
        for (int i = 0; i < height; i++) {
            std::vector<cpp::Console::Symbol> vecdsym;
            for (int j = 0; j < width; j++) {
                vecdsym.push_back(symbols[i][j]);
            }
            vecsyms.push_back(vecdsym);
        }
        unsigned int out[2];
        std::array<unsigned long int,2> ret = cpp::Console::FillConsole(vecsyms);
        out[0] = ret[0];
        out[1] = ret[1];
        return (int*)out;
    }

    // Symbol
        libexport void* Console_Symbol_Constuct(wchar_t character, char foreground = 7, char background = 0) {
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
            smb->~Symbol();
            delete smb;
        }
    // ~Symbol

// ~Console