#include <dynamic_library.h>
#include <unicode_conversion.hpp>

namespace cppimp {
    libimport void Console_Init(void);

    libimport unsigned long int* Console_FillScreen$ret2(void*** symbols, int height, int width);

    libimport int16_t Console_GetWindowWidth(void);

    libimport int16_t Console_GetWindowHeight(void);

    libimport void* Console_Symbol_Construct$smb(void* smb);

    libimport void* Console_Symbol_Construct$cfb(uniconv::unichar character, uint8_t foreground = 7, uint8_t background = 0);

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
} // namespace cppimp

namespace csimp {
    
    libimport uniconv::unichar** FileSystem_ImportText(uniconv::unichar* file);
    
    libimport void FileSystem_ExportText(uniconv::unichar* file, uniconv::unichar** content);

} // namespace csimp

