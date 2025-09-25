#ifndef __APPLE__
#include <cuchar>
#ifdef __CYGWIN32__
namespace std {
    inline size_t mbrtoc16 (char16_t *__restrict__ __pc16, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) noexcept(true) {return mbrtowc(__pc16, __s, __n, __p);}
    inline size_t c16rtomb(char *__restrict__ __s, char16_t __c16, mbstate_t *__restrict__ __ps) noexcept(true) {return wcrtomb(__s, __c16, __ps);}
    inline size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) noexcept(true) {return mbrtowc(__pc32, __s, __n, __p);}
    inline size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps) noexcept(true) {return wcrtomb(__s, __c32, __ps);}
}
#endif
#else

namespace std {
    size_t mbrtoc16 (char16_t *__restrict__ __pc16, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) noexcept(true);
    size_t c16rtomb(char *__restrict__ __s, char16_t __c16, mbstate_t *__restrict__ __ps) noexcept(true);
    size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) noexcept(true);
    size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps) noexcept(true);
}

#endif