#include "thin_string.h"

#include "allocator.h"
#include "keywords.h"
#include <stdlib.h>
#include <string.h>

ThinString ths_with_capacity_in(Allocator allocator, size_t cap) {
    ThinString ths = sqx_alloc(allocator, sizeof(ThinString_Header) + cap, alignof(ThinString_Header));

    ths->length = 0;
    ths->capacity = cap;
    ths->allocator = allocator;
    memset(ths->bytes, 0, THIN_STRING_DEFAULT_CAPACITY);

    return ths;
}

ThinString ths_from_in(Allocator allocator, const char *s) {
    size_t len = strlen(s);
    size_t cap = len + 1;

    ThinString ths = ths_with_capacity_in(allocator, cap > THIN_STRING_DEFAULT_CAPACITY ? cap : THIN_STRING_DEFAULT_CAPACITY);

    memcpy(ths->bytes, s, len);

    return ths;
}

void ths_reserve(ThinString *ths, size_t n) {
    size_t len = (*ths)->length + n;
    if (len > (*ths)->capacity) {
        size_t new_cap = (*ths)->capacity == 0 ? THIN_STRING_DEFAULT_CAPACITY : (*ths)->capacity;
        while (new_cap <= len) new_cap *= THIN_STRING_GROWTH_FACTOR;
        *ths = sqx_realloc((*ths)->allocator, *ths, sizeof(ThinString_Header) + new_cap, alignof(ThinString_Header));
        (*ths)->capacity = new_cap;
        memset(&(*ths)->bytes[(*ths)->length], 0, new_cap - (*ths)->length);
    }
}

void ths_reserve_exact(ThinString *ths, size_t n) {
    size_t new_cap = (*ths)->length + n;
    if (new_cap > (*ths)->capacity) {
        *ths = sqx_realloc((*ths)->allocator, *ths, sizeof(ThinString_Header) + new_cap, alignof(ThinString_Header));
        (*ths)->capacity = new_cap;
        memset(&(*ths)->bytes[(*ths)->length], 0, new_cap - (*ths)->length);
    }
}

void ths_append(ThinString *ths, const char *s) {
    size_t len = strlen(s);
    ths_appendn(ths, len, s);
}

void ths_appendn(ThinString *ths, size_t n, const char *s) {
    if ((*ths)->capacity <= (*ths)->length + n) {
        ths_reserve(ths, n);
    }
    memcpy(&(*ths)->bytes[(*ths)->length], s, n);
    (*ths)->length += n;
}

void ths_clear(ThinString ths) {
    memset(ths->bytes, 0, ths->length);
    ths->length = 0;
}

void ths_free(ThinString *ths) {
    free(*ths);
    *ths = NULL;
}

