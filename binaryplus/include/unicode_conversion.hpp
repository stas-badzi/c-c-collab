#pragma once

#include <string>
#include <cstdint>
#include <limits>
#include <string.h>
#include <math.h>

#include "utils/dllalloc.h"

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
    return (unichar)(GetUnsignedChar(utf8_code));
#else

    unsigned long utf8_size = utf8_code.length();
    unichar unicode = 0;

    for (unsigned p=0; p<utf8_size; ++p) {
        int bit_count = (p? 6: 8 - utf8_size - (utf8_size == 1? 0: 1)),
            shift = (p < utf8_size - 1? (6*(utf8_size - p - 1)): 0);

        for (int k=0; k<bit_count; ++k) {
            unicode += ((GetUnsignedChar(utf8_code[p]) & (1 << k)) << shift);
        }
    }

    return unicode;

#endif
}


inline utfchar UnicodeToUtf8(unichar unicode) {

#ifdef _WIN32
    return (utfchar)(GetDefaultChar(unicode));
#else

    utfchar s;

    if (unicode <= 0x7f)  /* 7F(16) = 127(10) */ {
        s = static_cast<char>(GetDefaultChar(unicode));

        return s;
    } else if (unicode <= 0x7ff)  /* 7FF(16) = 2047(10) */ {
        unsigned char c1 = 192, c2 = 128;

        for (int k=0; k<11; ++k) {
            if (k < 6) {
                c2 |= (unicode % 64) & (1 << k);
            } else {
                c1 |= (unicode >> 6) & (1 << (k - 6));
            }
        }

        s = GetDefaultChar(c1);
        s += GetDefaultChar(c2);

        return s;
    } else if (unicode <= 0xffff)  /* FFFF(16) = 65535(10) */ {
        unsigned char c1 = 224, c2 = 128, c3 = 128;

        for (int k=0; k<16; ++k) {
            if (k < 6) {
                c3 |= (unicode % 64) & (1 << k);
            } else if (k < 12) {
                c2 |= (unicode >> 6) & (1 << (k - 6));
            } else {
                c1 |= (unicode >> 12) & (1 << (k - 12));
            }
        }

        s = GetDefaultChar(c1);
        s += GetDefaultChar(c2);
        s += GetDefaultChar(c3);

        return s;
    } else if (unicode <= 0x1fffff)  /* 1FFFFF(16) = 2097151(10) */ {
        unsigned char c1 = 240, c2 = 128, c3 = 128, c4 = 128;

        for (int k=0; k<21; ++k)
        {
            if (k < 6)
                c4 |= (unicode % 64) & (1 << k);
            else if (k < 12)
                c3 |= (unicode >> 6) & (1 << (k - 6));
            else if (k < 18)
                c2 |= (unicode >> 12) & (1 << (k - 12));
            else
                c1 |= (unicode >> 18) & (1 << (k - 18));
        }

        s = GetDefaultChar(c1);
        s += GetDefaultChar(c2);
        s += GetDefaultChar(c3);
        s += GetDefaultChar(c4);

        return s;
    } else if (unicode <= 0x3ffffff)  /* 3FFFFFF(16) = 67108863(10) */ {
        ;  // Actually, there are no 5-bytes unicodes
    }
    else if (unicode <= 0x7fffffff)  /* 7FFFFFFF(16) = 2147483647(10) */ {
        ;  // Actually, there are no 6-bytes unicodes
    } else {
        ;  // Incorrect unicode (< 0 or > 2147483647)
    }

    return "";

#endif
}

#ifdef _WIN32
    inline utfstr WStringToNative(std::wstring wstr) { return wstr; }
    inline utfchar WCharToNative(wchar_t wchar) { return wchar; }
    inline std::wstring NativeToWString(utfstr utfstr) { return utfstr; }
    inline wchar_t NativeToWChar(utfchar utfchar) { return utfchar; }
    inline utfchar ReadUtfChar(utfcstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        if (bytes_read != nullptr) { *bytes_read = 1; }
        return str[offset];
    }
#else

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
#endif

    inline unichar* Utf8StringToUnicode (utfcstr utf8s) {
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (strlen(utf8s) + 1));
        size_t offset;
        for (size_t i = 0; i < strlen(utf8s); i += offset) {
            out[i] = Utf8ToUnicode(ReadUtfChar(utf8s, i, &offset));
        }
        out[strlen(utf8s)] = 0;
        return out;
    }

    inline utfstr UnicodeToUtf8String (unichar* unicodes) {
        utfstr out;
        for (int i = 0; unicodes[i] != 0; ++i) {
            out.append(to_string(UnicodeToUtf8(unicodes[i])));
        }
        __dllfree(unicodes);
        return out;
    }


} // namespace uniconv