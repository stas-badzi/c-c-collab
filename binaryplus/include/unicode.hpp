#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <string>
#include <string.h>
#include <vector>
#include <cstdint>

typedef uint32_t unicode_t;
#if __cplusplus < 202002L
typedef unsigned char char8_t; // so intelisense doesn't complain (it doesn't know it's c++20)
namespace std
{
    typedef basic_string<char8_t> u8string; // same here
} // namespace std

#endif

namespace unicode {

    // @param code unicode code point
    // @return utf32 character encoded in array of char32_t allocated with calloc
    inline char32_t* UnicodeToUTF32(unicode_t code) {
        char32_t* utf32 = (char32_t*)malloc(2* sizeof(char32_t));
        utf32[0] = code;
        utf32[1] = 0;
        return utf32;
    }

    // @param utf32 character encoded in array of char32_t
    // @return unicode code point
    inline unicode_t UTF32ToUnicode(const char32_t utf32[]) {
        return utf32[0];
    }


    // @param code unicode code point
    // @return utf16 character encoded in array of char16_t allocated with calloc
    inline char16_t* UnicodeToUTF16(unicode_t code) {
        char16_t* utf16; 
        if (code <= 0xd7ff  && code < 0x10000) {
            utf16 = (char16_t*)malloc(2* sizeof(char16_t));
            utf16[0] = code;
            utf16[1] = 0;
        } else if (code <= 0x10ffff) {
            utf16 = (char16_t*)malloc(3* sizeof(char16_t));
            utf16[0] = 0xd800 + ((code-0x10000) >> 10);
            utf16[1] = 0xdc00 + ((code-0x10000) & 0x3ff);
            utf16[2] = 0;
        } else {
            utf16 = (char16_t*)malloc(2* sizeof(char16_t));
            utf16[0] = 0xfffd;
            utf16[1] = 0;
        }
        return utf16;
    }

    // @param utf16 character encoded in array of char16_t
    // @param force_valid_codepoint if true, then if utf16 is an invalid unicode codepoint, then return 0xfffd
    // @return unicode code point
    inline unicode_t UTF16ToUnicode(const char16_t utf16[], bool force_valid_codepoint = true) {
        unicode_t code;
        if (utf16[0] <= 0xd7ff || utf16[0] >= 0xe000) {
            code = utf16[0];
        } else if (utf16[0] >= 0xd800 && utf16[1] >= 0xdc00) {
            code = 0x10000 + ((utf16[0] - 0xd800) << 10) + (utf16[1] - 0xdc00);
        } else if (force_valid_codepoint) {
            code = 0xfffd;
        } else {
            code = utf16[0];
        }
        return code;
    }
    
    // @param code unicode code point
    // @return utf16 character encoded in array of char16_t allocated with calloc
    inline char8_t* UnicodeToUTF8(unicode_t code) {
        char8_t* utf8;
        if (code <= 0x7f) {
            utf8 = (char8_t*)malloc(2 * sizeof(char8_t));
            utf8[0] = code;
            utf8[1] = 0;
        } else if (code <= 0x7ff) {
            utf8 = (char8_t*)malloc(3* sizeof(char8_t));
            utf8[0] = 0xc0 + (code >> 6);
            utf8[1] = 0x80 + ((code >> 0) & 0x3f);
            utf8[2] = 0;
        } else if (code <= 0xffff) {
            utf8 = (char8_t*)malloc(4* sizeof(char8_t));
            utf8[0] = 0xe0 + (code >> 12);
            utf8[1] = 0x80 + ((code >> 6) & 0x3f);
            utf8[2] = 0x80 + ((code >> 0) & 0x3f);
            utf8[3] = 0;
        } else if (code <= 0x10ffff) {
            utf8 = (char8_t*)malloc(5* sizeof(char8_t));
            utf8[0] = 0xf0 + (code >> 18);
            utf8[1] = 0x80 + ((code >> 12) & 0x3f);
            utf8[2] = 0x80 + ((code >> 6) & 0x3f);
            utf8[3] = 0x80 + (code & 0x3f);
            utf8[4] = 0;
        } else {
            utf8 = (char8_t*)malloc(4* sizeof(char8_t));
            // 0xfffd
            utf8[0] = 0xef;
            utf8[1] = 0x80 + ((0xfffd >> 6) & 0x3f);
            utf8[2] = 0x80 + ((0xfffd >> 0) & 0x3f);
            utf8[3] = 0;
        }
        return utf8;
    }

    // @param utf16 character encoded in array of char16_t
    // @param force_valid_codepoint if true, then if utf8 is an invalid unicode codepoint, then return 0xfffd
    // @return unicode code point
    inline unicode_t UTF8ToUnicode(const char8_t utf8[], bool force_valid_codepoint = true) {
        if (utf8[0] <= 0x7f) {
            return utf8[0];
        } else if (utf8[0] >= 0xc2 && utf8[0] <= 0xdf) {
            return (utf8[0] & 0x1f) << 6 | (utf8[1] & 0x3f);
        } else if (utf8[0] >= 0xe0 && utf8[0] <= 0xef) {
            return (utf8[0] & 0x0f) << 12 | (utf8[1] & 0x3f) << 6 | (utf8[2] & 0x3f);
        } else if (utf8[0] >= 0xf0 && utf8[0] <= 0xf4) {
            return (utf8[0] & 0x07) << 18 | (utf8[1] & 0x3f) << 12 | (utf8[2] & 0x3f) << 6 | (utf8[3] & 0x3f);
        } else if (force_valid_codepoint) {
            return 0xfffd;
        } else {
            return utf8[0];
        }
    }
    
    inline std::u32string UnicodeToUTF32String(unicode_t code[]) {
        std::u32string str;
        for (auto i = 0; code[i] != 0; i++) {
            str += UnicodeToUTF32(code[i]);
        }
        return str;
    }

    inline unicode_t* UTF32StringToUnicode(std::u32string str) {
        unicode_t* arr = (unicode_t*)malloc((str.length() + 1) * sizeof(unicode_t));
        for (auto i = 0; str[i] != U'\0'; i++) {
            str += UTF32ToUnicode(&str[i]);
        }
        arr[str.length()] = 0;
        return arr;
    }
    
    inline std::u16string UnicodeToUTF16String(unicode_t code[]) {
        std::u16string str;
        for (auto i = 0; code[i] != 0; i++) {
            str += UnicodeToUTF16(code[i]);
        }
        return str;
    }

    inline unicode_t* UTF16StringToUnicode(std::u16string str, bool force_valid_codepoint = true) {
        unicode_t* arr = (unicode_t*)malloc((str.length() + 1) * sizeof(char16_t));
        size_t length = 0;
        size_t size = str.length() + 1;
        for (auto i = 0; str[i] != u'\0'; ++i) {
            if (str[i] <= 0xd7ff || str[i] >= 0xe000) {
                arr[i] = str[i]; 
            } else if (str[i] >= 0xd800 && str[i + 1] >= 0xdc00) {
                arr[i] = 0x10000 + ((str[i] - 0xd800) << 10) + (str[i+1] - 0xdc00);
                i += 1;
                --size;
            } else if (force_valid_codepoint) {
                arr[i] = 0xfffd;
            } else {
                arr[i] = str[0];
            }
        }
        arr = (unicode_t*)realloc(arr, size * sizeof(char16_t));
        arr[size-1] = 0;
        return arr;
    }
    
    inline std::u8string UnicodeToUTF8String(unicode_t code[]) {
        std::u8string str;
        for (auto i = 0; code[i] != 0; i++) {
            str += UnicodeToUTF8(code[i]);
        }
        return str;
    }

    inline unicode_t* UTF8StringToUnicode(std::u8string str, bool force_valid_codepoint = true) {
        unicode_t* arr = (unicode_t*)malloc((str.length() + 1) * sizeof(char8_t));
        size_t length = 0;
        size_t size = str.length() + 1;
        for (auto i = 0; str[i] != u8'\0'; ++i) {
            if (str[i] <= 0x7f) {
                arr[i] = str[i];
            } else if (str[i] >= 0xc2 && str[i] <= 0xdf) {
                arr[i] = (str[i] & 0x1f) << 6 | (str[i+1] & 0x3f);
                ++i;
                --size;
            } else if (str[i] >= 0xe0 && str[i] <= 0xef) {
                arr[i] = (str[i] & 0x0f) << 12 | (str[i+1] & 0x3f) << 6 | (str[i+2] & 0x3f);
                ++i; ++i;
                --size; --size;
            } else if (str[i] >= 0xf0 && str[i] <= 0xf4) {
                arr[i] = (str[i] & 0x07) << 18 | (str[i+1] & 0x3f) << 12 | (str[i+2] & 0x3f) << 6 | (str[i+3] & 0x3f);
                ++i; ++i; ++i;
                --size; --size; --size;
            } else if (force_valid_codepoint) {
                arr[i] = 0xfffd;
            } else {
                arr[i] = str[0];
            }
        }
        arr = (unicode_t*)realloc(arr, size * sizeof(char8_t));
        arr[size-1] = 0;
        return arr;
    }
}

#endif // UNICODE_HPP