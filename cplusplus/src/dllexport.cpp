#include <dynamic_library.h>
#include <unicode_conversion.hpp>
#include <vector>

#include "Console.hpp"
#include "System.hpp"

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

// System
    libexport uniconv::unichar* System_GetRootPath(void) {
        return uniconv::Utf8StringToUnicode(cpp::System::GetRootPath());
    }
    
    libexport uniconv::unichar* System_ToNativePath(uniconv::unichar* arg1) {
        return uniconv::Utf8StringToUnicode(cpp::System::ToNativePath(uniconv::UnicodeToUtf8String(arg1)));
    }
    
    libexport nint System_AllocateMemory(uint32_t arg1) {
        return cpp::System::AllocateMemory(arg1);
    }

    libexport void System_FreeMemory(nint arg1) {
        return cpp::System::FreeMemory(arg1);
    }

    libexport nint System_MovePointer(nint arg1, int32_t arg2) {
        return cpp::System::MovePointer(arg1,arg2);
    }
    // ReadPointer<T>
        libexport int8_t System_ReadPointer$int8(nint arg1) {
            return cpp::System::ReadPointer<int8_t>(arg1);
        }

        libexport int16_t System_ReadPointer$int16(nint arg1) {
            return cpp::System::ReadPointer<int16_t>(arg1);
        }

        libexport int32_t System_ReadPointer$int32(nint arg1) {
            return cpp::System::ReadPointer<int32_t>(arg1);
        }

        libexport int64_t System_ReadPointer$int64(nint arg1) {
            return cpp::System::ReadPointer<int64_t>(arg1);
        }

        libexport uint8_t System_ReadPointer$uint8(nint arg1) {
            return cpp::System::ReadPointer<uint8_t>(arg1);
        }

        libexport uint16_t System_ReadPointer$uint16(nint arg1) {
            return cpp::System::ReadPointer<uint16_t>(arg1);
        }

        libexport uint32_t System_ReadPointer$uint32(nint arg1) {
            return cpp::System::ReadPointer<uint32_t>(arg1);
        }

        libexport uint64_t System_ReadPointer$uint64(nint arg1) {
            return cpp::System::ReadPointer<uint64_t>(arg1);
        }

        libexport bool System_ReadPointer$bool(nint arg1) {
            return cpp::System::ReadPointer<bool>(arg1);
        }

        libexport nint System_ReadPointer$nint(nint arg1) {
            return cpp::System::ReadPointer<nint>(arg1);
        }

        // offset = true

        libexport int8_t System_ReadPointer$int8$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int8_t>(arg1,arg2);
        }

        libexport int16_t System_ReadPointer$int16$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int16_t>(arg1,arg2);
        }

        libexport int32_t System_ReadPointer$int32$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int32_t>(arg1,arg2);
        }

        libexport int64_t System_ReadPointer$int64$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int64_t>(arg1,arg2);
        }

        libexport uint8_t System_ReadPointer$uint8$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint8_t>(arg1,arg2);
        }

        libexport uint16_t System_ReadPointer$uint16$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint16_t>(arg1,arg2);
        }

        libexport uint32_t System_ReadPointer$uint32$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint32_t>(arg1,arg2);
        }

        libexport uint64_t System_ReadPointer$uint64$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint64_t>(arg1,arg2);
        }

        libexport bool System_ReadPointer$bool$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<bool>(arg1,arg2);
        }

        libexport nint System_ReadPointer$nint$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<nint>(arg1,arg2);
        }
    // ~ReadPointer<T>

    
    // WritePointer<T>
        libexport void System_WritePointer$int8(nint arg1, int8_t arg2) {
            return cpp::System::WritePointer<int8_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int16(nint arg1, int16_t arg2) {
            return cpp::System::WritePointer<int16_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int32(nint arg1, int32_t arg2) {
            return cpp::System::WritePointer<int32_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int64(nint arg1, int64_t arg2) {
            return cpp::System::WritePointer<int64_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint8(nint arg1, uint8_t arg2) {
            return cpp::System::WritePointer<uint8_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint16(nint arg1, uint16_t arg2) {
            return cpp::System::WritePointer<uint16_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint32(nint arg1, uint32_t arg2) {
            return cpp::System::WritePointer<uint32_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint64(nint arg1, uint64_t arg2) {
            return cpp::System::WritePointer<uint64_t>(arg1,arg2);
        }

        libexport void System_WritePointer$bool(nint arg1, bool arg2) {
            return cpp::System::WritePointer<bool>(arg1,arg2);
        }

        libexport void System_WritePointer$nint(nint arg1, nint arg2) {
            return cpp::System::WritePointer<nint>(arg1,arg2);
        }

        // offset = true

        libexport void System_WritePointer$int8$ofs(nint arg1, int32_t arg3, int8_t arg2) {
            return cpp::System::WritePointer<int8_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int16$ofs(nint arg1, int32_t arg3, int16_t arg2) {
            return cpp::System::WritePointer<int16_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int32$ofs(nint arg1, int32_t arg3, int32_t arg2) {
            return cpp::System::WritePointer<int32_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int64$ofs(nint arg1, int32_t arg3, int64_t arg2) {
            return cpp::System::WritePointer<int64_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint8$ofs(nint arg1, int32_t arg3, uint8_t arg2) {
            return cpp::System::WritePointer<uint8_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint16$ofs(nint arg1, int32_t arg3, uint16_t arg2) {
            return cpp::System::WritePointer<uint16_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint32$ofs(nint arg1, int32_t arg3, uint32_t arg2) {
            return cpp::System::WritePointer<uint32_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint64$ofs(nint arg1, int32_t arg3, uint64_t arg2) {
            return cpp::System::WritePointer<uint64_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$bool$ofs(nint arg1, int32_t arg3, bool arg2) {
            return cpp::System::WritePointer<bool>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$nint$ofs(nint arg1, int32_t arg3, nint arg2) {
            return cpp::System::WritePointer<nint>(arg1,arg3,arg2);
        }

    // ~WritePointer<T>

// ~System