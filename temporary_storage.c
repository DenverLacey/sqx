#include "temporary_storage.h"

#include "allocator.h"
#include "attributes.h"
#include "keywords.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct TemporaryStorage {
    char *buffer;
    size_t size;
    size_t allocated;
    size_t high_water_mark;
    char *previous_allocation;
} T;

void tsinit(size_t size) {
    T.buffer = malloc(size);
    T.size = size;
    T.allocated = 0;
    T.high_water_mark = 0;
}

void tsdeinit(void) {
    free(T.buffer);
    T.buffer = NULL;
    T.size = 0;
    T.allocated = 0;
    T.high_water_mark = 0;
}

TSMark tsmark(void) {
    return (TSMark) {
        T.previous_allocation,
        T.allocated,
    };
}

void tsrollback(TSMark mark) {
    assert(mark.ptr >= (const void*)T.buffer && mark.ptr <= (const void*)T.previous_allocation);
    assert(mark.allocated <= T.allocated);
    T.previous_allocation = (char*)mark.ptr;
    T.allocated = mark.allocated;
}

void tsreset(void) {
    T.allocated = 0;
    T.high_water_mark = 0;
}

void *tsalloc(size_t size, size_t align) {
    align = align != 0 ? align : alignof(void*);
    uintptr_t p = (uintptr_t)T.buffer + T.allocated;
    p = ((p + (align - 1)) / align) * align;

    size_t new_allocated = (p - (uintptr_t)T.buffer) + size;
    if (new_allocated > T.size) {
        return NULL;
    }

    T.allocated = new_allocated;

    if (T.allocated > T.high_water_mark) {
        T.high_water_mark = T.allocated;
    }

    T.previous_allocation = (char*)p;
    return (void*)p;
}

void *tsrealloc(const void *ptr, size_t new_size, size_t align) {
    if (ptr != T.previous_allocation) {
        return tsalloc(new_size, align);
    }

    size_t new_allocated = ((uintptr_t)ptr - (uintptr_t)T.buffer) + new_size;
    if (new_allocated > T.size) {
        return NULL;
    }

    T.allocated = new_allocated;

    if (T.allocated > T.high_water_mark) {
        T.high_water_mark = T.allocated;
    }

    return (void*)ptr;
}

char *tsgetbuffer(void) {
    return T.buffer;
}

size_t tsgetsize(void) {
    return T.size;
}

size_t tsgetallocated(void) {
    return T.allocated;
}

size_t tsgetwatermark(void) {
    return T.high_water_mark;
}

static void *tsallocatorfunc(UNUSED void *ap, AllocatorMode mode, const void *ptr, size_t size, size_t align) {
    switch (mode) {
        case AllocatorMode_ALLOC:
            return tsalloc(size, align);
        case AllocatorMode_REALLOC:
            return tsrealloc(ptr, size, align);
        case AllocatorMode_ALLOC_AND_ZERO: {
            void *p = tsalloc(size, align);
            memset(p, 0, size);
            return p;
        } break;
        case AllocatorMode_FREE:
            break;
        case AllocatorMode_CLEAR:
            tsreset();
            break;
        default:
            break;
    }
    return NULL;
}

Allocator tsallocator(void) {
    return (Allocator) {
        .allocator_data = NULL,
        .allocator_func = tsallocatorfunc,
    };
}

