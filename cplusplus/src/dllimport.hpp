#pragma once

#include <stdint.h>

#include <dynamic_library.h>
#include <unicode_conversion.hpp>
#include <control_heap.h>

#include "Console.hpp"


namespace cppimp {
    // Symbol
        inline void* Console_Symbol_Construct$smb(cpp::Console::Symbol* src) {
            void* out = (void*) new cpp::Console::Symbol(*src);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }


        inline void* Console_Symbol_Construct$cfb(uniconv::unichar character, uint8_t foreground = 7, uint8_t background = 0) {
            void* out = new cpp::Console::Symbol(uniconv::UnicodeToUtf8(character), foreground, background);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }

        #ifdef _WIN32
            inline void* Console_Symbol_Construct$atr(uint8_t attribute) {
                void* out = (void*) new cpp::Console::Symbol(attribute);
            #ifdef _DEBUG
                __save$SYMBOLS(out);
            #endif
                return out;
            }
            
            inline void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, uint8_t attribute) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                smb->SetAttribute(attribute);
            }

            inline uint8_t Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                return smb->GetAttribute();
            }
        #endif

        inline void Console_Symbol_character$set(cpp::Console::Symbol* smb, uniconv::unichar character) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->character = uniconv::UnicodeToUtf8(character);
        }

        inline uniconv::unichar Console_Symbol_character$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return uniconv::Utf8ToUnicode(smb->character);
        }

        inline void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, uint8_t foreground) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->foreground = foreground;
        }

        inline uint8_t Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->foreground;
        }

        inline void Console_Symbol_background$set(cpp::Console::Symbol* smb, uint8_t background) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->background = background;
        }

        inline uint8_t Console_Symbol_background$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->background;
        }

        inline void Console_Symbol_Destruct(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __free$SYMBOLS(smb);
        #endif
            delete smb;
        }

        inline void* Console_Symbol_operator$eq(cpp::Console::Symbol* cp, cpp::Console::Symbol* src) {
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

    // Camera
        libimport void* Game_Camera_Construct(int height, int width, void* symptr);
        libimport void* Game_Camera_ViewportCenter(cpp::Game::Camera* cameraptr);
        libimport void Game_Camera_DrawTexture(void* textureptr, cpp::Game::MatrixPosition* centerptr, cpp::Game::Camera* cameraptr);

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