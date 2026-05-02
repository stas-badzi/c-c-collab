#pragma once

#include <stdint.h>

#include <dynamic_library.h>
#include <unicode_conversion.hpp>

#include "Console.hpp"


namespace cppimp {
    // Symbol
        inline void* Console_Symbol_Construct$smb(cpp::Console::Symbol* src) {
            void* out = (void*) new cpp::Console::Symbol(*src);
            return out;
        }


        inline void* Console_Symbol_Construct$cfb(uniconv::unichar character, uint8_t foreground = 7, uint8_t background = 0) {
            void* out = new cpp::Console::Symbol(uniconv::UnicodeToNative(character), foreground, background);
            return out;
        }

        #ifdef _WIN32
            inline void* Console_Symbol_Construct$atr(uint8_t attribute) {
                void* out = (void*) new cpp::Console::Symbol(attribute);
                return out;
            }
            
            inline void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, uint8_t attribute) {
                smb->SetAttribute(attribute);
            }

            inline uint8_t Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
                return smb->GetAttribute();
            }
        #endif

        inline void Console_Symbol_character$set(cpp::Console::Symbol* smb, uniconv::unichar character) {
        smb->character = uniconv::UnicodeToNative(character);
        }

        inline uniconv::unichar Console_Symbol_character$get(cpp::Console::Symbol* smb) {
        return uniconv::NativeToUnicode(smb->character);
        }

        inline void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, uint8_t foreground) {
        smb->foreground = foreground;
        }

        inline uint8_t Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
        return smb->foreground;
        }

        inline void Console_Symbol_background$set(cpp::Console::Symbol* smb, uint8_t background) {
        smb->background = background;
        }

        inline uint8_t Console_Symbol_background$get(cpp::Console::Symbol* smb) {
        return smb->background;
        }

        inline void Console_Symbol_Destruct(cpp::Console::Symbol* smb) {
            delete smb;
        }

        inline void* Console_Symbol_operator$eq(cpp::Console::Symbol* cp, cpp::Console::Symbol* src) {
            void* out = (void*) new cpp::Console::Symbol( (*cp) = (*src) );
            return out;
        }
    // ~Symbol

    // actually from c# but who cares
        libimport void Console_Symbol_ReverseColors(cpp::Console::Symbol* sym);
    //
}

namespace csimp {
    
    libimport uniconv::unichar** TextureSystem_ImportText(uniconv::unichar* file);
    
    libimport void TextureSystem_ExportText(uniconv::unichar* file, uniconv::unichar** content);

    libimport void* TextureSystem_TextureFromFile(uniconv::unichar* arg1);

    libimport void TextureSystem_FileFromTexture(uniconv::unichar* filepathPtr, void* texturePtr, bool recycle);

    libimport void TextureSystem_DrawTextureToScreen(int x, int y, void* texturePtr, void* screenPtr);

    libimport void SoundSystem_PlaySound(uniconv::unichar* filepathPtr, bool wait);

} // namespace csimp