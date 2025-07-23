#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "attributes.h"

#include <stddef.h>

typedef enum AllocatorMode {
    AllocatorMode_ALLOC,
    AllocatorMode_ALLOC_AND_ZERO,
    AllocatorMode_REALLOC,
    AllocatorMode_FREE,
    AllocatorMode_CLEAR,
} AllocatorMode;

typedef void *(*AllocatorFunc)(void *allocator_data, AllocatorMode mode, const void *ptr, size_t size, size_t align);

typedef struct Allocator {
    void *        allocator_data;
    AllocatorFunc allocator_func;
} Allocator;

INLINE void *sqx_alloc(Allocator allocator, size_t size, size_t align);
INLINE void *sqx_calloc(Allocator allocator, size_t size, size_t align);
INLINE void *sqx_realloc(Allocator allocator, const void *ptr, size_t size, size_t align);
INLINE void  sqx_free(Allocator allocator, const void *ptr, size_t size);
INLINE void  sqx_clear(Allocator allocator);

#endif // _ALLOCATOR_H_

