#if !defined(__APPLE__) && !defined(__CYGWIN32__)
#include <uchar.h>
#else
#include <wchar.h>
#if !defined(__cplusplus)
typedef unsigned short char16_t;
typedef unsigned int char32_t;
#endif
size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p);
size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps);
#ifdef __CYGWIN32__
static size_t mbrtoc16(char16_t *__restrict__ __pc16, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) {return mbrtowc((wchar_t*)__pc16, __s, __n, __p);}
static size_t c16rtomb(char *__restrict__ __s, char16_t __c16, mbstate_t *__restrict__ __ps) {return wcrtomb(__s, __c16, __ps);}
#else
size_t mbrtoc16(char16_t *__restrict__ __pc16, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p);
size_t c16rtomb(char *__restrict__ __s, char16_t __c16, mbstate_t *__restrict__ __ps);
#endif
#endif
