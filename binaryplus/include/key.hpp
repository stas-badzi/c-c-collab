#ifdef _WIN32
#include <windows/key.hpp>
#elif CYGWIN
#include <windows/key.hpp>
#elif __linux__
#include <linux/key.hpp>
#elif __APPLE__
#include <apple/key.hpp>
#elif __BSD__
#include <bsd/key.hpp>
#else
#error "Unknown platform"
#endif