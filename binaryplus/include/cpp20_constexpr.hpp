#if __cplusplus >= 202000L && !defined(__CYGWIN32__)
#define cpp20_constexpr constexpr
#else
#define cpp20_constexpr
#endif