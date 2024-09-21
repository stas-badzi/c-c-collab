#include <stdint.h>

#include <dynamic_library.h>
#include <unicode_conversion.hpp>

#include "defines.h"

namespace cppimp {
    libimport void Console_Init(void);

    libimport void Console_Fin(void);

    libimport void Console_Sleep(double seconds);

    libimport int Console_HandleKeyboard(void);

    libimport bool Console_IsKeyDown(int arg1);

    libimport bool Console_IsKeyToggled(int arg1);

    libimport int Console_KeyPressed(void);

    libimport int Console_KeyReleased(void);

    libimport void Console_HandleMouseAndFocus(void);

    libimport void* Console_GetMouseStatus(void);

    libimport uint8_t* Console_MouseButtonClicked$ret2(void);

    libimport uint8_t Console_MouseButtonReleased(void);

    libimport bool Console_IsFocused(void);

    libimport unsigned long int* Console_FillScreen$ret2(void*** symbols, int height, int width);

    libimport int16_t Console_GetWindowWidth(void);

    libimport int16_t Console_GetWindowHeight(void);

    libimport void* Console_Symbol_Construct$smb(void* smb);

    libimport void* Console_Symbol_Construct$cfb(uniconv::unichar character, uint8_t foreground, uint8_t background);

    #ifdef _WIN32
        libimport void* Console_Symbol_Construct$atr(uint8_t attribute);
        
        libimport void Console_Symbol_SetAttribute(void* smb, uint8_t attribute);

        libimport uint8_t Console_Symbol_GetAttribute(void* smb);
    #endif

    libimport void Console_Symbol_character$set(void* smb, uniconv::unichar character);

    libimport uniconv::unichar Console_Symbol_character$get(void* smb);

    libimport void Console_Symbol_foreground$set(void* smb, uint8_t foreground);

    libimport uint8_t Console_Symbol_foreground$get(void* smb);

    libimport void Console_Symbol_background$set(void* smb, uint8_t background);

    libimport uint8_t Console_Symbol_background$get(void* smb);

    libimport void Console_Symbol_Destruct(void* smb);

    libimport void* Console_Symbol_operator$eq(void* cp, void* src);

    libimport uniconv::unichar* System_GetRootPath(void);
    
    libimport uniconv::unichar* System_ToNativePath(uniconv::unichar* arg1);
    
    libimport nint System_AllocateMemory(size_t arg1);

    libimport void System_FreeMemory(nint arg1);

    libimport nint System_MovePointer(nint arg1, int32_t arg2);
    // ReadPointer<T>
        libimport int8_t System_ReadPointer$int8(nint arg1);

        libimport int16_t System_ReadPointer$int16(nint arg1);

        libimport int32_t System_ReadPointer$int32(nint arg1);

        libimport int64_t System_ReadPointer$int64(nint arg1);

        libimport uint8_t System_ReadPointer$uint8(nint arg1);

        libimport uint16_t System_ReadPointer$uint16(nint arg1);

        libimport uint32_t System_ReadPointer$uint32(nint arg1);

        libimport uint64_t System_ReadPointer$uint64(nint arg1);

        libimport bool System_ReadPointer$bool(nint arg1);

        libimport nint System_ReadPointer$nint(nint arg1);

        // offset = true

        libimport int8_t System_ReadPointer$int8$ofs(nint arg1, int32_t arg2);

        libimport int16_t System_ReadPointer$int16$ofs(nint arg1, int32_t arg2);

        libimport int32_t System_ReadPointer$int32$ofs(nint arg1, int32_t arg2);

        libimport int64_t System_ReadPointer$int64$ofs(nint arg1, int32_t arg2);

        libimport uint8_t System_ReadPointer$uint8$ofs(nint arg1, int32_t arg2);

        libimport uint16_t System_ReadPointer$uint16$ofs(nint arg1, int32_t arg2);

        libimport uint32_t System_ReadPointer$uint32$ofs(nint arg1, int32_t arg2);

        libimport uint64_t System_ReadPointer$uint64$ofs(nint arg1, int32_t arg2);

        libimport bool System_ReadPointer$bool$ofs(nint arg1, int32_t arg2);

        libimport nint System_ReadPointer$nint$ofs(nint arg1, int32_t arg2);
    // ~ReadPointer<T>

    
    // WritePointer<T>
        libimport void System_WritePointer$int8(nint arg1, int8_t arg2);

        libimport void System_WritePointer$int16(nint arg1, int16_t arg2);

        libimport void System_WritePointer$int32(nint arg1, int32_t arg2);

        libimport void System_WritePointer$int64(nint arg1, int64_t arg2);

        libimport void System_WritePointer$uint8(nint arg1, uint8_t arg2);

        libimport void System_WritePointer$uint16(nint arg1, uint16_t arg2);

        libimport void System_WritePointer$uint32(nint arg1, uint32_t arg2);

        libimport void System_WritePointer$uint64(nint arg1, uint64_t arg2);

        libimport void System_WritePointer$bool(nint arg1, bool arg2);

        libimport void System_WritePointer$nint(nint arg1, nint arg2);

        // offset = true

        libimport void System_WritePointer$int8$ofs(nint arg1, int32_t arg3, int8_t arg2);

        libimport void System_WritePointer$int16$ofs(nint arg1, int32_t arg3, int16_t arg2);

        libimport void System_WritePointer$int32$ofs(nint arg1, int32_t arg3, int32_t arg2);

        libimport void System_WritePointer$int64$ofs(nint arg1, int32_t arg3, int64_t arg2);

        libimport void System_WritePointer$uint8$ofs(nint arg1, int32_t arg3, uint8_t arg2);

        libimport void System_WritePointer$uint16$ofs(nint arg1, int32_t arg3, uint16_t arg2);

        libimport void System_WritePointer$uint32$ofs(nint arg1, int32_t arg3, uint32_t arg2);

        libimport void System_WritePointer$uint64$ofs(nint arg1, int32_t arg3, uint64_t arg2);

        libimport void System_WritePointer$bool$ofs(nint arg1, int32_t arg3, bool arg2);

        libimport void System_WritePointer$nint$ofs(nint arg1, int32_t arg3, nint arg2);


} // namespace CppImp

namespace csimp {
    
    libimport uniconv::unichar** FileSystem_ImportText(uniconv::unichar* file);
    
    libimport void FileSystem_ExportText(uniconv::unichar* file, uniconv::unichar** content);

    libimport void* FileSystem_TextureFromFile(uniconv::unichar* arg1);

    libimport void FileSystem_FileFromTexture(uniconv::unichar* filepathPtr, void* texturePtr, bool recycle);

    libimport void FileSystem_DrawTextureToScreen(int x, int y, void* texturePtr, void* screenPtr);

    libimport void FileSystem_PlaySound(uniconv::unichar* filepathPtr, bool wait);

} // namespace csimp

