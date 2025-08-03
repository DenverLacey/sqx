#ifndef _TEMPORARY_STORAGE_H_
#define _TEMPORARY_STORAGE_H_

#include "allocator.h"
#include "keywords.h"
#include "macro_ops.h"

#include <stddef.h>

typedef struct TSMark {
    const void* ptr;
    size_t      allocated;
} TSMark;

void    tsinit(size_t size);
void    tsdeinit(void);
TSMark  tsmark(void);
void    tsrollback(TSMark mark);
void    tsreset(void);
void *  tsalloc(size_t size, size_t align);
void *  tsrealloc(const void *ptr, size_t new_size, size_t align);
#define tsnew(ty__) ((ty__*)tsalloc(sizeof(ty__), alignof(ty__)))
#define tsnewn(ty__, n__) ((ty__*)tsalloc(sizeof(ty__) * (n__), alignof(ty__)))

char * tsgetbuffer(void);
size_t tsgetsize(void);
size_t tsgetallocated(void);
size_t tsgetwatermark(void);

Allocator tsallocator(void);

#ifdef __GNUC__
#define tsmake(ty__, ...) ({                           \
    ty__ *GENSYM(tsmake_result_ln) = tsnew(ty__);      \
    *GENSYM(tsmake_result_ln) = (ty__){ __VA_ARGS__ }; \
    GENSYM(tsmake_result_ln);                          \
})
#endif

#endif // _TEMPORARY_STORAGE_H_

