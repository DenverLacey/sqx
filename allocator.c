#include "allocator.h"

#include <stdlib.h>

INLINE void *sqx_alloc(Allocator allocator, size_t size, size_t align) {
    if (allocator.allocator_func == NULL) {
        return malloc(size);
    }
    return allocator.allocator_func(allocator.allocator_data, AllocatorMode_ALLOC, NULL, size, align);
}

INLINE void *sqx_calloc(Allocator allocator, size_t size, size_t align) {
    if (allocator.allocator_func == NULL) {
        return calloc(1, size);
    }
    return allocator.allocator_func(allocator.allocator_data, AllocatorMode_ALLOC_AND_ZERO, NULL, size, align);
}

INLINE void *sqx_realloc(Allocator allocator, const void *ptr, size_t size, size_t align) {
    if (allocator.allocator_func == NULL) {
        return realloc((void*)ptr, size);
    }
    return allocator.allocator_func(allocator.allocator_data, AllocatorMode_REALLOC, ptr, size, align);
}

INLINE void sqx_free(Allocator allocator, const void *ptr, size_t size) {
    if (allocator.allocator_func == NULL) {
        free((void*)ptr);
    } else {
        allocator.allocator_func(allocator.allocator_data, AllocatorMode_FREE, ptr, size, 0);
    }
}

INLINE void sqx_clear(Allocator allocator) {
    if (allocator.allocator_func == NULL) {
        return; // NOTE: Is this the best thing to do in this situation?
    }
    allocator.allocator_func(allocator.allocator_data, AllocatorMode_CLEAR, NULL, 0, 0);
}

