#include <cwchar>
#include <cerrno>
#include <cstdlib>

#ifndef __unused
    #define __unused(x) x
#endif

namespace std {
    size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __unused(__p)) noexcept(true) {
        
        if ( __s == nullptr )
            return 0;
        /* something like this (it doesn't really matter for utf-32)
        if (ps->__count > 0)
            --ps->__count;
            *pc32 = ps->__value.__wch;
            return -3;
        */

        bool complete = false;
        size_t csize = 0;
        char32_t out = 0;
        size_t i = 0;
        
        while (i < __n) {
            unsigned char si = __s[i];
            if (i) {
                if (si <= 0x7f || si >= 0xc0) { errno = EILSEQ; return -1; }
                out |= (si & (unsigned char)0x7f) << (6 * (csize-i-1));
            } else {
                if (!si) {
                    __pc32[i] = U'\0';
                    return 0;
                } else if (si <= (unsigned char)0x7f) {
                    out |= si;
                    csize = 1;
                } else if (si >= (unsigned char)0xc2 && si <= (unsigned char)0xdf) {
                    csize = 2;
                    out |= (si & (unsigned char)0x1f) << 6;
                } else if (si >= (unsigned char)0xe0 && si <= (unsigned char)0xef) {
                    csize = 3;
                    out |= (si & (unsigned char)0x0f) << 12;
                } else if (si >= (unsigned char)0xf0 && si <= (unsigned char)0xf4) {
                    csize = 4;
                    out |= (si & (unsigned char)0x07) << 18;
                } else { errno = EILSEQ; return -1; }
            }
            if (csize == ++i) {
                __pc32[0] = out;
                return csize;
            }
        }
        return -2;
    }

    size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __unused(__ps)) noexcept(true) {
        if (__s == nullptr)
            return 1;
        int amount = 0;
        char utf8[4];
        if (__c32 <= 0x7f) {
            amount = 1;
            utf8[0] = __c32;
        } else if (__c32 <= 0x7ff) {
            amount = 2;
            utf8[0] = 0xc0 + (__c32 >> 6);
            utf8[1] = 0x80 + ((__c32 >> 0) & 0x3f);
        } else if (__c32 <= 0xffff) {
            amount = 3;
            utf8[0] = 0xe0 + (__c32 >> 12);
            utf8[1] = 0x80 + ((__c32 >> 6) & 0x3f);
            utf8[2] = 0x80 + ((__c32 >> 0) & 0x3f);
        } else if (__c32 <= 0x10ffff) {
            amount = 4;
            utf8[0] = 0xf0 + (__c32 >> 18);
            utf8[1] = 0x80 + ((__c32 >> 12) & 0x3f);
            utf8[2] = 0x80 + ((__c32 >> 6) & 0x3f);
            utf8[3] = 0x80 + (__c32 & 0x3f);
        } else {
            // 0xfffd
            amount = 3;
            utf8[0] = 0xef;
            utf8[1] = '\x80' + ((0xfffd >> 6) & 0x3f);
            utf8[2] = '\x80' + ((0xfffd >> 0) & 0x3f);
        }

        if (amount > MB_CUR_MAX) amount = MB_CUR_MAX;
        for (int i = 0; i < amount; i++)
            __s[i] = utf8[i];
        return amount;
    }
    //c32rtomb;
}