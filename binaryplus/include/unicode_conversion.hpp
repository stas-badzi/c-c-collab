#pragma once

#include <string>
#include <cstdint>
#include <limits>
#include <string.h>
#include <math.h>
#include <iterator>

#include "utils/dllalloc.h"

#ifdef _WIN32

#else
    #include <locale>
    #include <codecvt>
    #include <bitset>
    #include <string_view>
    #include <exception>
    #include "cuchar.hpp"
#endif

namespace uniconv {

    typedef uint32_t unichar;

    inline constexpr unsigned char GetUnsignedChar(char val) {
        return val;
    }

    inline constexpr char GetDefaultChar(unsigned char val) {
        return (std::numeric_limits<char>::is_signed) ? (char)val+2*INT8_MIN : val;
    }

    inline constexpr unsigned int GetUnsignedWChar(wchar_t val) {
        return (std::numeric_limits<wchar_t>::is_signed) ? (unsigned int)(((long long)val-2LL*WCHAR_MIN)%(1LL+WCHAR_MAX-WCHAR_MIN)) : val;
    }

    inline constexpr wchar_t GetDefaultWChar(unsigned int val) {
        return (std::numeric_limits<wchar_t>::is_signed) ? (wchar_t)val+2LL*WCHAR_MIN : val;
    }

#ifdef _WIN32
    #define to_nstring(val) to_wstring(val)
    #define N(val) L##val
    typedef wchar_t utfchar;
    typedef std::wstring nstring;
    typedef const wchar_t* utfcstr;
    inline constexpr nstring to_string(utfchar val) {
        nstring out; 
        out.push_back(val);
        return out;
    }
#else
    #define to_nstring(val) to_string(val)
    #define N(val) val
    typedef std::string utfchar;
    typedef std::string nstring;
    typedef const char* utfcstr;
    inline constexpr nstring to_string(utfchar val) { return val; }
#endif

inline unichar NativeToUnicode(utfchar utf8_code) {
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


inline utfchar UnicodeToNative(unichar unicode) {
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

inline unichar* U16StringToUnicode(std::u16string utf16s) {
    unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (utf16s.size() + 1));
    for (size_t i = 0; i < utf16s.size(); ++i)
        out[i] = static_cast<unichar>(utf16s[i]);
    out[utf16s.size()] = 0;
    return out;
}

inline std::u16string UnicodeToU16String(unichar* unicodes) {
    std::u16string out;
    for (int i = 0; unicodes[i] != 0; ++i)
        out.push_back(static_cast<char16_t>(unicodes[i]));
    __dllfree(unicodes);
    return out;
}

inline constexpr char16_t UnicodeToChar16(unichar unicode) {
    return static_cast<char16_t>(unicode);
}

inline constexpr unichar Char16ToUnicode(char16_t char16) {
    return static_cast<unichar>(char16);
}

#ifdef _WIN32
    inline constexpr std::u16string NativeToU16String(std::wstring str) {
        std::u16string out;
        out.reserve(str.size());
        copy(str.begin(), str.end(), back_inserter(out));
        return out;
    }

    inline constexpr std::wstring U16StringToNative(std::u16string u16str) {
        std::wstring out;
        out.reserve(u16str.size());
        copy(u16str.begin(), u16str.end(), back_inserter(out));
        return out;
    }

    inline constexpr std::wstring WStringToNative(std::wstring wstr) { return wstr; }

    inline constexpr std::wstring NativeToWString(std::wstring wstr) { return wstr; }

    inline constexpr std::u16string WStringToU16String(std::wstring wstr) { return NativeToU16String(wstr); }
    
    inline constexpr std::wstring U16StringToWString(std::u16string u16str) { return U16StringToNative(u16str); }

    inline constexpr char16_t WCharToChar16(wchar_t wc) { return wc; }
    inline constexpr wchar_t Char16ToWChar(char16_t c16) { return c16; }

    // don't use this function anymore
    inline utfchar ReadUtfChar(utfcstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        if (bytes_read != nullptr) { *bytes_read = 1; }
        return str[offset];
    }
    
    inline unichar* NativeStringToUnicode(utfcstr utf8s) {
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (wcslen(utf8s) + 1));
        size_t offset;
        for (size_t i = 0; i < wcslen(utf8s); i++) out[i] = utf8s[i];
        out[wcslen(utf8s)] = 0;
        return out;
    }

    inline nstring UnicodeToNativeString(unichar* unicodes) {
        nstring out;
        for (int i = 0; unicodes[i] != 0; ++i) {
            out.push_back(unicodes[i]);
        }
        __dllfree(unicodes);
        return out;
    }

#else
    inline std::u16string NativeToU16String(std::string str) {
        using namespace std;
        std::u16string out;
        mbstate_t state = mbstate_t();
        char* c = (char*)str.c_str();
        char* end = c + str.size();
        while (c < end) {
            char16_t c16;
            size_t siz = mbrtoc16(&c16, c, end-c, &state);
            switch (siz) {
            case static_cast<size_t>(-1):
                exit(1);
            case static_cast<size_t>(-2):
                continue;
            case static_cast<size_t>(-3):
                out.push_back(c16);
                continue;
            default:
                out.push_back(c16);
                c += siz;
                continue;
            }
        }
        // handling surrogate pair at end of string
        char empty = 0;
        char16_t c16;
        size_t siz = mbrtoc16(&c16, &empty, 1, &state);
        if (siz == static_cast<size_t>(-3))
            out.push_back(c16);
        return out;
    }

    inline std::string U16StringToNative(std::u16string u16str) {
        using namespace std;
        std::string out;
        std::mbstate_t state = std::mbstate_t();
        char16_t* c16 = (char16_t*)u16str.c_str();
        char16_t* end = c16 + u16str.size();
        --c16; while (++c16 < end) {
            char ch[4] = {0};
            size_t siz = c16rtomb(ch, *c16, &state);
            if (siz != (size_t)-1)
                for (char c8 : std::string_view{ch, siz})
                    out.push_back(c8);
            else exit(1);
        }
        return out;
    }

    inline std::string WStringToNative(std::wstring wstr) {
        using namespace std;
        std::string out;
        mbstate_t state = mbstate_t();
        wchar_t* wc = (wchar_t*)wstr.c_str();
        wchar_t* end = wc + wstr.size();
        while (wc < end) {
            char ch[4] = {0};
            size_t siz = wcrtomb(ch, *wc, &state);
            if (siz != (size_t)-1)
                for (char c8 : std::string_view{ch, siz})
                    out.push_back(c8);
            else exit(1);
            wc += siz;
        }
        return out;
    }

    inline std::wstring NativeToWString(std::string str) {
        using namespace std;
        std::wstring out;
        mbstate_t state = mbstate_t();
        char* c = (char*)str.c_str();
        char* end = c + str.size();
        while (c < end) {
            wchar_t wc;
            size_t siz = mbrtowc(&wc, c, end-c, &state);
            switch (siz) {
            case static_cast<size_t>(-1):
                exit(1);
            case static_cast<size_t>(-2):
                continue;
            case static_cast<size_t>(-3):
                out.push_back(wc);
                continue;
            default:
                out.push_back(wc);
                c += siz;
                continue;
            }
        }
        // handling surrogate pair at end of string
        char empty = 0;
        wchar_t wc;
        size_t siz = mbrtowc(&wc, &empty, 1, &state);
        if (siz == static_cast<size_t>(-3))
            out.push_back(wc);
        return out;
    }

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

    inline unichar* NativeStringToUnicode (utfcstr utf8s) {
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

    inline nstring UnicodeToNativeString (unichar* unicodes) {
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

    inline constexpr std::u16string WStringToU16String(std::wstring wstr) {
        std::u16string out;
        for (size_t i = 0; i < wstr.size(); i++)
            out.push_back(static_cast<char16_t>(wstr[i]));
        return out;
    }

    inline constexpr char16_t WCharToChar16(wchar_t wchar) {
        return static_cast<char16_t>(wchar);
    }

    inline constexpr std::wstring U16StringToWString(std::u16string u16str) {
        std::wstring out;
        for (size_t i = 0; i < u16str.size(); i++)
            out.push_back(static_cast<wchar_t>(u16str[i]));
        return out;
    }

    inline constexpr wchar_t Char16ToWChar(char16_t utfchar) {
        return static_cast<wchar_t>(utfchar);
    }

#endif

} // namespace uniconv