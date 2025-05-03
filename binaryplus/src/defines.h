#pragma once

#define nint void*

#ifdef _WIN32
    #include <windows.h>
namespace cpp {
    typedef HANDLE fd_t;
#else
namespace cpp {
    typedef int fd_t;
#endif

    typedef struct rw_pipe {
        fd_t read;
        fd_t write;
    } rw_pipe_t;
}