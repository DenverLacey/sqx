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
    char *previoud_allocation; // TODO: Use this to implement realloc
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

size_t tsmark(void) {
    return T.allocated;
}

void tsrollback(size_t mark) {
    assert(mark <= T.allocated);
    T.allocated = mark;
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

    return (void*)p;
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

static void *tsallocatorfunc(UNUSED void *ap, AllocatorMode mode, UNUSED const void *ptr, size_t size, size_t align) {
    switch (mode) {
        case AllocatorMode_ALLOC:
        case AllocatorMode_REALLOC:
            return tsalloc(size, align);
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

