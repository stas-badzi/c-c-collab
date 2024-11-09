#pragma once

#include <string>
#include <cstdint>
#include <limits>
#include <string.h>
#include <math.h>

#include "utils/dllalloc.h"
#include "unicode.hpp"

#ifdef _WIN32

#else
    #include <locale>
    #include <codecvt>
    #include <bitset>
    #include <exception>
#endif

namespace uniconv {

    typedef uint32_t unichar;

    inline unsigned char GetUnsignedChar(char val) {
        return val;
    }

    inline char GetDefaultChar(unsigned char val) {
        return (std::numeric_limits<char>::is_signed) ? (char)val+2*INT8_MIN : val;
    }

    inline unsigned int GetUnsignedWChar(wchar_t val) {
        return (std::numeric_limits<wchar_t>::is_signed) ? (unsigned int)(((long long)val-2LL*WCHAR_MIN)%(1LL+WCHAR_MAX-WCHAR_MIN)) : val;
    }

    inline wchar_t GetDefaultWChar(unsigned int val) {
        return (std::numeric_limits<wchar_t>::is_signed) ? (wchar_t)val+2LL*WCHAR_MIN : val;
    }

#ifdef _WIN32
    typedef wchar_t utfchar;
    typedef std::wstring utfstr;
    typedef const wchar_t* utfcstr;
    inline utfstr to_string(utfchar val) {
        utfstr out; 
        out.push_back(val);
        return out;
    }
#else
    typedef std::string utfchar;
    typedef std::string utfstr;
    typedef const char* utfcstr;
    inline utfstr to_string(utfchar val) { return val; }
#endif

inline unichar Utf8ToUnicode(utfchar utf8_code) {
    
#ifdef _WIN32
    return utf8_code;
#else
    std::u8string u8code;
    for (auto &&c : utf8_code) u8code.push_back(GetUnsignedChar(c));
    char16_t* ret = unicode::UnicodeToUTF16(unicode::UTF8ToUnicode(u8code.c_str()));
    unichar utf16 = ret[0];
    free(ret);
    return utf16;
#endif
}


inline utfchar UnicodeToUtf8(unichar unicode) {

#ifdef _WIN32
    return unicode;
#else
    char16_t param[2] = u"X";
    param[0] = unicode;
    unicode_t func = unicode::UTF16ToUnicode(param);
    auto ret = unicode::UnicodeToUTF8(func);
    std::u8string u8code = ret;
    free(ret);
    std::string utf8_code;
    for (auto &&c : u8code) utf8_code.push_back(GetDefaultChar(c));
    
    return utf8_code;
#endif
}

#ifdef _WIN32
    inline utfstr WStringToNative(std::wstring wstr) { return wstr; }
    inline utfchar WCharToNative(wchar_t wchar) { return wchar; }
    inline std::wstring NativeToWString(utfstr utfstr) { return utfstr; }
    inline wchar_t NativeToWChar(utfchar utfchar) { return utfchar; }
    // don't use this function anymore
    inline utfchar ReadUtfChar(utfcstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        if (bytes_read != nullptr) { *bytes_read = 1; }
        return str[offset];
    }
    
    inline unichar* Utf8StringToUnicode (utfcstr utf8s) {
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (strlen(utf8s) + 1));
        size_t offset;
        for (size_t i = 0; i < strlen(utf8s); i++) out[i] = utfs[i];
        out[strlen(utf8s)] = 0;
        return out;
    }

    inline utfstr UnicodeToUtf8String (unichar* unicodes) {
        utfstr out;
        for (int i = 0; unicodes[i] != 0; ++i) {
            out.append(unicodes[i]);
        }
        __dllfree(unicodes);
        return out;
    }

#else
    // don't use this function anymore
    inline utfchar ReadUtfChar(utfcstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        utfchar out;
        std::bitset<8> char_bits(GetUnsignedChar(str[offset])); {
            int a1=char_bits[0],a2=char_bits[1],a3=char_bits[2],a4=char_bits[3],a5=char_bits[4],a6=char_bits[5],a7=char_bits[6],a8=char_bits[7];
            char_bits[0] = a8; char_bits[1] = a7; char_bits[2] = a6; char_bits[3] = a5; char_bits[4] = a4; char_bits[5] = a3; char_bits[6] = a2; char_bits[7] = a1; 
        }
    #ifdef _DEBUG
        int Q1=char_bits[0],Q2=char_bits[1],Q3=char_bits[2],Q4=char_bits[3],Q5=char_bits[4],Q6=char_bits[5],Q7=char_bits[6],Q8=char_bits[7];
    #endif
        uint8_t bytes;
        if (char_bits[0] == 0) {
            // 0xxxxxxx
            bytes = 0;
        } else
        // 1xxxxxxx
        if (char_bits[1] == 0) {
            // 10xxxxxx
            throw std::runtime_error("Invalid UTF-8 character");
        } else
        // 11xxxxxx
        if (char_bits[2] == 0) {
        // 110xxxxx
            bytes = 1;
        } else // 111xxxxx
        if (char_bits[3] == 0) {
            // 1110xxxx
            bytes = 2;
        } else // 1111xxxx
        if (char_bits[4] == 0) {
            // 11110xxx
            bytes = 3;
        } else {
            throw std::runtime_error("Invalid UTF-8 character");
        }

        out.push_back(str[offset]);

        for (uint8_t i = 1; i <= bytes; i++) {
            char_bits = std::bitset<8>(GetUnsignedChar(str[offset + i])); {
                int a1=char_bits[0],a2=char_bits[1],a3=char_bits[2],a4=char_bits[2],a5=char_bits[4],a6=char_bits[5],a7=char_bits[6],a8=char_bits[7];
                char_bits[0] = a8; char_bits[1] = a7; char_bits[2] = a6; char_bits[3] = a5; char_bits[4] = a4; char_bits[5] = a3; char_bits[6] = a2; char_bits[7] = a1; 
            }
        #ifdef _DEBUG
            int Q1=char_bits[0],Q2=char_bits[1],Q3=char_bits[2],Q4=char_bits[3],Q5=char_bits[4],Q6=char_bits[5],Q7=char_bits[6],Q8=char_bits[7];
        #endif

            if (char_bits[0] != 1 || char_bits[1] != 0) {
                throw std::runtime_error("Invalid UTF-8 character");
            }
            out.push_back(str[offset + i]);
        }


        if (bytes_read != nullptr) { *bytes_read = ++bytes; }

        return out;
    }

    inline utfstr WStringToNative(std::wstring wstr) {
        utfstr out = "";
        for (unsigned i = 0; i < wstr.size(); ++i) {
            out.append(UnicodeToUtf8(GetUnsignedWChar(wstr[i])));
        }
        return out;
    }

    inline utfchar WCharToNative(wchar_t wchar) {
        return UnicodeToUtf8(GetUnsignedWChar(wchar));
    }

    inline std::wstring NativeToWString(utfstr utfstr) {
        std::wstring out = L"";
        size_t locread = 0;
        for (size_t read = 0; read < utfstr.size(); read += locread) {
            out.push_back(GetDefaultWChar(Utf8ToUnicode(ReadUtfChar(utfstr.c_str(), read, &locread))));
        }
        return out;
    }

    inline wchar_t NativeToWChar(utfchar utfchar) {
        return GetDefaultWChar(Utf8ToUnicode(utfchar));
    }

    inline unichar* Utf8StringToUnicode (utfcstr utf8s) {
        std::u8string param;
        for (size_t i = 0; i < strlen(utf8s); ++i) {
            param.push_back(GetUnsignedChar(utf8s[i]));
        }
        auto func = unicode::UTF8StringToUnicode(param);
        auto ret = unicode::UnicodeToUTF16String(func);
        free(func);
        
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (ret.size() + 1));
        size_t offset;
        for (size_t i = 0; i < strlen(utf8s); i++) out[i] = ret[i];
        out[ret.size()] = 0;
        return out;
    }

    inline utfstr UnicodeToUtf8String (unichar* unicodes) {
        std::u16string param;
        for (int i = 0; unicodes[i] != 0; ++i) param.push_back(unicodes[i]);
        __dllfree(unicodes);
        unicode_t* func = unicode::UTF16StringToUnicode(param);
        std::u8string ret = unicode::UnicodeToUTF8String(func);
        free(func);
        utfstr out;
        for (auto &&c : ret) out.push_back(GetDefaultChar(c));
        return out;
    }

#endif

} // namespace uniconv