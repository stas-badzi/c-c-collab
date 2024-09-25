#pragma once

#include "../dynamic_library.h"

namespace dll {
    libimport void* System_AllocateMemory(size_t arg1);
    libimport void System_FreeMemory(void* arg1);
}

inline void __dllfree(void* _Block) {
    dll::System_FreeMemory(_Block);
}

inline void* __dllalloc(size_t _Size) {
    return dll::System_AllocateMemory(_Size);
}