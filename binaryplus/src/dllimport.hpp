#include <dynamic_library.h>

namespace cppimp {
    libimport void Console_Init(void);

    libimport unsigned long int* Console_FillScreen$ret2(void*** symbols, int height, int width);

    libimport int Console_GetWindowWidth(void);

    libimport int Console_GetWindowHeight(void);

    libimport void* Console_Symbol_Constuct$smb(void* sym);

    libimport void* Console_Symbol_Constuct$cfb(wchar_t character, char foreground = 7, char background = 0);

    #ifdef _WIN32
        libimport void* Console_Symbol_Constuct$atr(char attribute);
        
        libimport void Console_Symbol_SetAttribute(void* smb, char attribute);

        libimport char Console_Symbol_GetAttribute(void* smb);
    #endif

    libimport void Console_Symbol_character$set(void* smb, wchar_t character);

    libimport char Console_Symbol_character$get(void* smb);

    libimport void Console_Symbol_foreground$set(void* smb, wchar_t foreground);

    libimport char Console_Symbol_foreground$get(void* smb);

    libimport void Console_Symbol_background$set(void* smb, wchar_t background);

    libimport char Console_Symbol_background$get(void* smb);

    libimport void Console_Symbol_Destruct(void* smb);

    libimport void* Console_Symbol_operator$eq(void* cp, void* src);
} // namespace cppimp
