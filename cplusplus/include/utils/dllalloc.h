#include <stdlib.h>
#include "control_heap.h"

inline void __dllfree(void* _Block) {
    __free$ALLOCATIONS(_Block);
    free(_Block);
}

inline void* __dllalloc(size_t _Size) {
    void* out = malloc(_Size);
    __save$ALLOCATIONS(out,_Size);
    return out;
}