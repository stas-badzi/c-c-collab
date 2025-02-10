#ifndef __APPLE__
#include <cuchar>
#else

namespace std {
    size_t mbrtoc32 (char32_t *__restrict__ __pc32, const char *__restrict__ __s, size_t __n, mbstate_t *__restrict__ __p) noexcept(true);
    size_t c32rtomb(char *__restrict__ __s, char32_t __c32, mbstate_t *__restrict__ __ps) noexcept(true);
}

#endif