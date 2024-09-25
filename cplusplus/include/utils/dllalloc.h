#pragma once

#include "control_heap.h"
#include <stdlib.h>

inline void __dllfree(void* _Block) {
#ifdef _DEBUG
    __free$ALLOCATIONS(_Block);
#endif
    free(_Block);
}

inline void* __dllalloc(size_t _Size) {
    void* out = malloc(_Size);
#ifdef _DEBUG
    __save$ALLOCATIONS(out,_Size);
#endif
    return out;
}