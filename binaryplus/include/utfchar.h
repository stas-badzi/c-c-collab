#ifndef __APPLE__
#include <uchar.h>
#ifdef __CYGWIN32__
size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p);
size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps);
static size_t mbrtoc16(char16_t *__restrict__ __pc16, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) {return mbrtowc(__pc16, __s, __n, __p);}
static size_t c16rtomb(char *__restrict__ __s, char16_t __c16, mbstate_t *__restrict__ __ps) {return wcrtimb(__s, __c16, __ps);}
#endif
#else
size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p);
size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps);
#endif