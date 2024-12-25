#include "dllimport.hpp"

int sub(int type);

#ifdef __cplusplus
    extern "C"
#endif
    int Console_sub(int arg1) {
        return sub(arg1);
    }