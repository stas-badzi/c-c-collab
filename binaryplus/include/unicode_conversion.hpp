#pragma once

#include <string>
#include <cstdint>

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
#ifdef _WIN32
    typedef wchar_t utfchar;
    typedef std::wstring utfstr;
    inline utfstr to_string(utfchar val) {
        utfstr out; 
        out.push_back(val);
        return out;
    }
#else
    typedef std::string utfchar;
    typedef std::string utfstr;
    inline utfstr to_string(utfchar val) { return val; }
#endif

inline unichar Utf8ToUnicode(utfchar utf8_code) {
    
#ifdef _WIN32
    return (unichar)(utf8_code);
#else

    unsigned long utf8_size = utf8_code.length();
    unichar unicode = 0;

    for (unsigned p=0; p<utf8_size; ++p) {
        int bit_count = (p? 6: 8 - utf8_size - (utf8_size == 1? 0: 1)),
            shift = (p < utf8_size - 1? (6*(utf8_size - p - 1)): 0);

        for (int k=0; k<bit_count; ++k) {
            unicode += ((utf8_code[p] & (1 << k)) << shift);
        }
    }

    return unicode;

#endif
}


inline utfchar UnicodeToUtf8(unichar unicode) {

#ifdef _WIN32
    return (utfchar)(unicode);
#else

    utfchar s;

    if (unicode >= 0 and unicode <= 0x7f)  /* 7F(16) = 127(10) */ {
        s = static_cast<char>(unicode);

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

        s = c1;
        s += c2;

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

        s = c1;
        s += c2;
        s += c3;

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

        s = c1;
        s += c2;
        s += c3;
        s += c4;

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
    inline utfchar ReadUtfChar(utfstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        if (bytes_read != nullptr) { *bytes_read = 1; }
        return str[offset];
    }
#else

    inline utfchar ReadUtfChar(utfstr str, size_t offset = 0, size_t* bytes_read = nullptr) {
        utfchar out;
        std::bitset<8> char_bits;
        for(int i = 0; i < 8; i++) {
            char_bits[i] = ((unsigned char)(str[offset]) >> i) & 1;
        }
        uint8_t bytes;
        if (char_bits[0] == 0) {
            // 0xxxxxxx
            bytes = 0;
        }
        // 1xxxxxxx
        if (char_bits[1] == 0) {
            // 10xxxxxx
            throw std::runtime_error("Invalid UTF-8 character");
        }
        // 11xxxxxx
        if (char_bits[2] == 0) {
        // 110xxxxx
            bytes = 1;
        } else // 111xxxxx
        if (char_bits[3] == 0) {
            // 1110xxxx
            bytes = 2;
        } else // 1111xxxx
        if (char_bits[3] == 0) {
            // 11110xxx
            bytes = 3;
        } else {
            throw std::runtime_error("Invalid UTF-8 character");
        }

        out.push_back(str[offset]);

        for (uint8_t i = 1; i <= bytes; i++) {
            for(int l = 0; l < 8; l++) {
                char_bits[i] = ((unsigned char)(str[offset + l]) >> l) & 1;
            }
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
        for (int i = 0; i < wstr.size(); ++i) {
            out.append(UnicodeToUtf8((unichar)(wstr[i])));
        }
        return out;
    }

    inline utfchar WCharToNative(wchar_t wchar) {
        return UnicodeToUtf8((uint32_t)(wchar));
    }

    inline std::wstring NativeToWString(utfstr utfstr) {
        std::wstring out = L"";
        size_t locread;
        for (size_t read = 0; read < utfstr.size(); read += locread) {
            out.push_back((wchar_t)(Utf8ToUnicode(ReadUtfChar(utfstr, read, &locread))));
        }
        return out;
    }

    inline wchar_t NativeToWChar(utfchar utfchar) {
        return (wchar_t)(Utf8ToUnicode(utfchar));
    }
#endif

    inline unichar* Utf8StringToUnicode (utfstr utf8s) {
        unichar* out = (unichar*)__dllalloc(sizeof(unichar) * (utf8s.size() + 1));
        size_t offset;
        for (size_t i = 0; i < utf8s.size(); i += offset) {
            out[i] = Utf8ToUnicode(ReadUtfChar(utf8s, i, &offset));
        }
        out[utf8s.size()] = 0;
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