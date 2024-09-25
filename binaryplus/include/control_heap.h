#pragma once

#include <stdlib.h>
#include "dynamic_library.h"

// control_heap
    libimport void ControlHeap__save$ALLOCATIONS(void* arg1, unsigned long arg2);
    libimport void ControlHeap__free$ALLOCATIONS(void* arg1);
    libimport void ControlHeap__check$ALLOCATIONS(void* arg1, unsigned long arg2);
    libimport void ControlHeap__save$SYMBOLS(void* arg1);
    libimport void ControlHeap__free$SYMBOLS(void* arg1);
    libimport void ControlHeap__check$SYMBOLS(void* arg1);

inline void __save$ALLOCATIONS(void* start, unsigned long length = 1) {
    ControlHeap__save$ALLOCATIONS(start,length);
}

inline void __free$ALLOCATIONS(void* where) {
    ControlHeap__free$ALLOCATIONS(where);
}

inline void __check$ALLOCATIONS(void* start, unsigned long length = 1) {
    ControlHeap__check$ALLOCATIONS(start,length);
}

inline void __save$SYMBOLS(void* sym) {
    ControlHeap__save$SYMBOLS(sym);
}

inline void __free$SYMBOLS(void* sym) {
    ControlHeap__free$SYMBOLS(sym);
}

inline void __check$SYMBOLS(void* sym) {
    ControlHeap__check$SYMBOLS(sym);
}