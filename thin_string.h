#ifndef _THIN_STRING_H_
#define _THIN_STRING_H_

#include "allocator.h"
#include "attributes.h"
#include <stddef.h>

#define THIN_STRING_GROWTH_FACTOR (2)
#define THIN_STRING_DEFAULT_CAPACITY (16)

typedef struct ThinString_Header {
    Allocator allocator;
    size_t length, capacity;
    char bytes[];
} ThinString_Header;

typedef ThinString_Header *ThinString;

#define ths_new() ths_new_in((Allocator){0})
#define ths_new_in(a__) ths_with_capacity_in(a__, THIN_STRING_DEFAULT_CAPACITY)

#define ths_with_capacity(n__) ths_with_capacity_in((Allocator){0}, n__)
ThinString ths_with_capacity_in(Allocator allocator, size_t cap);

#define ths_from(s__) ths_from_in((Allocator){0}, s__)
ThinString ths_from_in(Allocator allocator, const char *s);

void ths_reserve(ThinString *ths, size_t n);
void ths_reserve_exact(ThinString *ths, size_t n);

INLINE void ths_append(ThinString *ths, const char *s);
void ths_appendn(ThinString *ths, size_t n, const char *s);

void ths_clear(ThinString ths);
void ths_free(ThinString *ths);

#endif // _THIN_STRING_H_

