#pragma once

#include <stdint.h>

#include <dynamic_library.h>
#include <unicode_conversion.hpp>
#include <control_heap.h>

#include "Console.hpp"


namespace cppimp {
    // Symbol
        void* Console_Symbol_Construct$smb(cpp::Console::Symbol* src) {
            void* out = (void*) new cpp::Console::Symbol(*src);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }


        void* Console_Symbol_Construct$cfb(uniconv::unichar character, uint8_t foreground = 7, uint8_t background = 0) {
            void* out = new cpp::Console::Symbol(uniconv::UnicodeToUtf8(character), foreground, background);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }

        #ifdef _WIN32
            void* Console_Symbol_Construct$atr(uint8_t attribute) {
                void* out = (void*) new cpp::Console::Symbol(attribute);
            #ifdef _DEBUG
                __save$SYMBOLS(out);
            #endif
                return out;
            }
            
            void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, uint8_t attribute) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                smb->SetAttribute(attribute);
            }

            uint8_t Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                return smb->GetAttribute();
            }
        #endif

        void Console_Symbol_character$set(cpp::Console::Symbol* smb, uniconv::unichar character) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->character = uniconv::UnicodeToUtf8(character);
        }

        uniconv::unichar Console_Symbol_character$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return uniconv::Utf8ToUnicode(smb->character);
        }

        void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, uint8_t foreground) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->foreground = foreground;
        }

        uint8_t Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->foreground;
        }

        void Console_Symbol_background$set(cpp::Console::Symbol* smb, uint8_t background) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->background = background;
        }

        uint8_t Console_Symbol_background$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->background;
        }

        void Console_Symbol_Destruct(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __free$SYMBOLS(smb);
        #endif
            delete smb;
        }

        void* Console_Symbol_operator$eq(cpp::Console::Symbol* cp, cpp::Console::Symbol* src) {
        #ifdef _DEBUG
            __check$SYMBOLS(src);
            __check$SYMBOLS(cp);
        #endif
            void* out = (void*) new cpp::Console::Symbol( (*cp) = (*src) );
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }
    // ~Symbol
}

namespace csimp {
    
    libimport uniconv::unichar** FileSystem_ImportText(uniconv::unichar* file);
    
    libimport void FileSystem_ExportText(uniconv::unichar* file, uniconv::unichar** content);

    libimport void* FileSystem_TextureFromFile(uniconv::unichar* arg1);

    libimport void FileSystem_FileFromTexture(uniconv::unichar* filepathPtr, void* texturePtr, bool recycle);

    libimport void FileSystem_DrawTextureToScreen(int x, int y, void* texturePtr, void* screenPtr);

    libimport void FileSystem_PlaySound(uniconv::unichar* filepathPtr, bool wait);

} // namespace csimp