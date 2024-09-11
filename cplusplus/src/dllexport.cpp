#include <dynamic_library.h>
#include <unicode_conversion.hpp>
#include <vector>

#include "Console.hpp"

using namespace uniconv;

// Console

    libexport void Console_Init(void) {
        cpp::Console::Init();
    }

    libexport void Console_Fin(void) {
        cpp::Console::Fin();
    }

    libexport void Console_Sleep(double seconds) {
        cpp::Console::Sleep(seconds);
    }

    libexport void Console_HandleKeyboard(void) {
        cpp::Console::HandleKeyboard();
    }

    libexport bool Console_IsKeyDown(int arg1) {
        return cpp::Console::IsKeyDown(arg1);
    }

    libexport bool Console_IsKeyToggled(int arg1) {
        return cpp::Console::IsKeyToggled(arg1);
    }

    libexport int Console_KeyPressed(void) {
        return cpp::Console::KeyPressed();
    }

    libexport int Console_KeyReleased(void) {
        return cpp::Console::KeyReleased();
    }

    libexport void Console_HandleMouseAndFocus(void) {
        return cpp::Console::HandleMouseAndFocus();
    }

    libexport void* Console_GetMouseStatus(void) {
        void* out = new cpp::Console::MouseStatus(cpp::Console::GetMouseStatus());
        return out;
    }

    libexport uint8_t* Console_MouseButtonClicked$ret2(void) {
        std::pair<uint8_t, uint8_t> func = cpp::Console::MouseButtonClicked();
        uint8_t* ret = new uint8_t[2]{func.first, func.second};
        return ret;
    }

    libexport uint8_t Console_MouseButtonReleased(void) {
        return cpp::Console::MouseButtonReleased();
    }

    libexport bool Console_IsFocused(void) {
        return cpp::Console::IsFocused();
    }

    libexport unsigned long int* Console_FillScreen$ret2(cpp::Console::Symbol*** symbols, int height, int width) {
        std::vector<std::vector<cpp::Console::Symbol> > vecsyms;
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

    libexport int16_t Console_GetWindowWidth(void) {
        return cpp::Console::GetWindowWidth();
    }

    libexport int16_t Console_GetWindowHeight(void) {
        return cpp::Console::GetWindowHeight();
    }

    // Symbol
        libexport void* Console_Symbol_Construct$smb(cpp::Console::Symbol* src) {
            return (void*) new cpp::Console::Symbol(*src);
        }


        libexport void* Console_Symbol_Construct$cfb(unichar character, uint8_t foreground = 7, uint8_t background = 0) {
            return (void*) new cpp::Console::Symbol(UnicodeToUtf8(character), foreground, background);
        }

        #ifdef _WIN32
            libexport void* Console_Symbol_Construct$atr(uint8_t attribute) {
                return (void*) new cpp::Console::Symbol(attribute);
            }
            
            libexport void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, uint8_t attribute) {
                smb->SetAttribute(attribute);
            }

            libexport uint8_t Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
                return smb->GetAttribute();
            }
        #endif

        libexport void Console_Symbol_character$set(cpp::Console::Symbol* smb, unichar character) {
            smb->character = UnicodeToUtf8(character);
        }

        libexport unichar Console_Symbol_character$get(cpp::Console::Symbol* smb) {
            return Utf8ToUnicode(smb->character);
        }

        libexport void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, uint8_t foreground) {
            smb->foreground = foreground;
        }

        libexport uint8_t Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
            return smb->foreground;
        }

        libexport void Console_Symbol_background$set(cpp::Console::Symbol* smb, uint8_t background) {
            smb->background = background;
        }

        libexport uint8_t Console_Symbol_background$get(cpp::Console::Symbol* smb) {
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