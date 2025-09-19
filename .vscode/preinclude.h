#pragma once
#ifdef __linux__
#define _GNU_SOURCE
#endif

#ifdef __CYGWIN__
#ifdef __linux__
#warning CYGWINS NOT LINUX
#undef __linux__
#endif
#endif