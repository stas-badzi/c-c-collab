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
    #include <cuchar>
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
    std::mbstate_t state{}; 
    char32_t utf32;
    const char* utf8 = utf8_code.c_str();
    auto siz = std::mbrtoc32(&utf32,utf8,strlen(utf8),&state);
    return utf32;
#endif
}


inline utfchar UnicodeToUtf8(unichar unicode) {

#ifdef _WIN32
    return unicode;
#else
    std::mbstate_t state{};
    char32_t utf32 = unicode;
    char* temp = new char[4];
    std::size_t len = std::c32rtomb(temp, utf32, &state);
    delete[] temp;
    std::string mbstr(len,' ');
    std::c32rtomb(&mbstr[0], utf32, &state);
    return mbstr;
#endif
}

#ifdef _WIN32
    inline utfstr WStringToNative(std::wstring wstr) { return wstr; }
    inline utfchar WCharToNative(wchar_t wchar) { return wchar; }
    inline std::wstring NativeToWString(utfcstr str) { return utfstr(str); }
    inline wchar_t NativeToWChar(utfchar utfchar) { return utfchar; }
    // don't use this function anymore
    inline utfchar ReadUtfChar(utfcstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        if (bytes_read != nullptr) { *bytes_read = 1; }
        return str[offset];
    }
    
    inline unichar* Utf8StringToUnicode (utfcstr utf8s) {
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (wstrlen(utf8s) + 1));
        size_t offset;
        for (size_t i = 0; i < wstrlen(utf8s); i++) out[i] = utfs[i];
        out[wstrlen(utf8s)] = 0;
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

    inline unichar* Utf8StringToUnicode (utfcstr utf8s) {
        std::mbstate_t state{};
        size_t length = strlen(utf8s);
        int offset = 0;
        std::u32string str;
        const char* ptr = utf8s, *end = ptr + length + 1;
        while (ptr < end) {
            char32_t utf32;
            auto siz = std::mbrtoc32(&utf32,ptr,end-ptr,&state);
            if (siz <= 0) break;
            str.push_back(utf32);
            ptr += siz;
        }
        
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (str.size() + 1));
        for (size_t i = 0; i < str.size(); i++) out[i] = str[i];
        out[str.size()] = 0;
        return out;
    }

    inline utfstr UnicodeToUtf8String (unichar* unicodes) {
        std::mbstate_t state{};
        
        std::string out;
        for (int i = 0; unicodes[i] != 0; ++i) {
            char32_t utf32 = unicodes[i];
            char* temp = new char[4];
            std::size_t len = std::c32rtomb(temp, utf32, &state);
            delete[] temp;
            std::string mbstr(len,' ');
            std::c32rtomb(&mbstr[0], utf32, &state);
            out.append(mbstr);
        }
        __dllfree(unicodes);
        return out;
    }

    inline utfstr WStringToNative(std::wstring wstr) {
        unichar* uninput = (unichar*)__dllalloc(sizeof(unichar) * (wstr.size() + 1) );
        for (size_t i = 0; i < wstr.size(); ++i)
            uninput[i] = wstr[i];
        uninput[wstr.size()] = 0;
        return UnicodeToUtf8String(uninput);
    }

    inline utfchar WCharToNative(wchar_t wchar) {
        return UnicodeToUtf8(wchar);
    }

    inline std::wstring NativeToWString(utfcstr utf8str) {
        auto ret =  Utf8StringToUnicode(utf8str);
        std::wstring out = L"";
        for (size_t i = 0; ret[i] != 0; i++)
            out.push_back(ret[i]);
        __dllfree(ret);
        return out;
    }

    inline wchar_t NativeToWChar(utfchar utfchar) {
        return Utf8ToUnicode(utfchar);
    }

#endif

} // namespace uniconv