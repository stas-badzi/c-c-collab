#pragma once

#ifdef _DEBUG

#include <stdlib.h>
#include <limits.h>
#include "utils/cextern.h"

cextern void** ALLOCATIONS;
cextern unsigned long __size$ALLOCATIONS;
cextern void** SYMBOLS;
cextern unsigned long __size$SYMBOLS;

inline void __save$ALLOCATIONS(void* start, unsigned long length = 1) {
    if (length == 0) length = 1;
    if (start == 0x0) throw("pointer is nullptr");
    unsigned long old_size = __size$ALLOCATIONS;
    __size$ALLOCATIONS += length + 1;
    if (old_size) {
        void** tmp = (void**)realloc(ALLOCATIONS,(__size$ALLOCATIONS) * sizeof(void*));
        if (tmp) ALLOCATIONS = tmp;
    } else ALLOCATIONS = (void**)malloc((__size$ALLOCATIONS) * sizeof(void*));

    for (unsigned long i = 0; i < length; i++) {
        ALLOCATIONS[old_size + i] = (unsigned char*)start + i;
    }
    ALLOCATIONS[old_size + length] = (void*)0;
}

inline void __free$ALLOCATIONS(void* where) {
    if (where == 0x0) throw("pointer is nullptr");
    int found = 0;
    int amount = 1;
    for (unsigned long i = 0; i < __size$ALLOCATIONS; i++) {
        if (found > 0) {
            ALLOCATIONS[i-amount] = ALLOCATIONS[i];
        } else if (found < 0) {
            ++amount;
            if (!ALLOCATIONS[i]) found = 1;
        } else if (ALLOCATIONS[i] == where) {
            found = -1;
        }
    }

    if (found != 1) throw("not found");
    
    __size$ALLOCATIONS -= amount;
    if (__size$ALLOCATIONS) {
        void** tmp = (void**)realloc(ALLOCATIONS,(__size$ALLOCATIONS) * sizeof(void*));
        if (tmp) ALLOCATIONS = tmp;
    } else if (-__size$ALLOCATIONS) throw("too much to remove");
    else free(ALLOCATIONS);
}

inline void __check$ALLOCATIONS(void* start, unsigned long length = 1) {
    if (start == 0x0) throw("pointer is nullptr");
    unsigned long first = -1;
    for (unsigned long i = 0; i < __size$ALLOCATIONS; i++) {
        if (ALLOCATIONS[i] == start) {
            first = i;
        } else if (!ALLOCATIONS[i] && first != ULONG_MAX) {
            if (i - first >= length) return;
            throw("allocated space too short");
        }
    }
    throw("pointer not found");
}

inline void __save$SYMBOLS(void* sym) {
    if (sym == 0x0) throw("pointer is nullptr");
    unsigned long old_size = __size$SYMBOLS;
    ++__size$SYMBOLS;
    if (old_size) {
        void** tmp = (void**)realloc(SYMBOLS,(__size$SYMBOLS) * sizeof(void*));
        if (tmp) SYMBOLS = tmp;
    } else SYMBOLS = (void**)malloc((__size$SYMBOLS) * sizeof(void*));
    
    SYMBOLS[old_size] = (void**)sym;
}

inline void __free$SYMBOLS(void* sym) {
    if (sym == 0x0) throw("pointer is nullptr");
    int found = 0;
    for (unsigned long i = 0; i < __size$SYMBOLS; i++) {
        if (found) SYMBOLS[i-1] = SYMBOLS[i];
        else if (SYMBOLS[i] == sym) found = 1;
    }

    if (!found) throw("not found");
    
    if (__size$SYMBOLS) {
        --__size$SYMBOLS;
        if (!__size$SYMBOLS) return free(SYMBOLS);
        void** tmp = (void**)realloc(SYMBOLS,(__size$SYMBOLS) * sizeof(void*));
        if (tmp) SYMBOLS = tmp;
    } else throw("nothing to remove");
}

inline void __check$SYMBOLS(void* sym) {
    if (sym == 0x0) throw("pointer is nullptr");
    for (unsigned long i = 0; i < __size$SYMBOLS; i++) {
        if (SYMBOLS[i] == sym) return;
    }
    throw("symbol not found");
}

#endif