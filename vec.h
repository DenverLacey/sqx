#ifndef _VEC_H_
#define _VEC_H_

#include "allocator.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define VEC_DO_IDENT_(name__) Vec_ ## name__
#define VEC_IDENT_(name__) VEC_DO_IDENT_(name__)

#define DEFINE_VEC_FOR_TYPE_WITH_NAME(ty__, name__) \
    typedef struct VEC_IDENT_(name__) {             \
        Allocator allocator;                        \
        size_t length, capacity;                    \
        ty__ *items;                                \
    } VEC_IDENT_(name__)

#define DEFINE_VEC_FOR_TYPE(ty__) DEFINE_VEC_FOR_TYPE_WITH_NAME(ty__, ty__)

#define VEC_GROWTH_FACTOR (2)

#define vec_with_capacity_using_name(ty__, tyname__, n__) (VEC_IDENT_(tyname__)) { \
    .allocator = {0},                                                              \
    .length = 0,                                                                   \
    .capacity = n__,                                                               \
    .items = malloc(n__ * sizeof(ty__))                                            \
}

#define vec_with_capacity_using_name_in(ty__, tyname__, n__, allocator__) (VEC_IDENT_(tyname__)) { \
    .allocator = (allocator__),                                                                    \
    .length = 0,                                                                                   \
    .capacity = (n__),                                                                             \
    .items = sqx_alloc((allocator__), ((n__) * sizeof(tyname__)), alignof(tyname__)),              \
}

#define vec_with_capacity(ty__, n__) vec_with_capacity_using_name(ty__, ty__, n__)
#define vec_with_capacity_in(ty__, n__, allocator__) vec_with_capacity_using_name_in(ty__, ty__, n__, allocator__)

#define vec_reserve(v__, n__) do {                                                                                            \
    size_t _len = (v__)->length + (n__);                                                                                      \
    if (_len > (v__)->capacity) {                                                                                             \
        size_t new_size = (v__)->capacity == 0 ? 1 : (v__)->capacity;                                                         \
        while (new_size < _len) new_size *= VEC_GROWTH_FACTOR;                                                                \
        (v__)->items = sqx_realloc((v__)->allocator, (v__)->items, new_size * sizeof(*(v__)->items), alignof(*(v__)->items)); \
        (v__)->capacity = new_size;                                                                                           \
    }                                                                                                                         \
} while (0)

#define vec_reserve_exact(v__, n__) do {                                                                                  \
    size_t _len = (v__)->len + (n__);                                                                                     \
    if (_len > (v__)->capacity) {                                                                                         \
        (v__)->items = sqx_realloc((v__)->allocator, (v__)->items, _len * sizeof(*(v__)->items), alignof(*(v__)->items)); \
        (v__)->capacity = _len;                                                                                           \
    }                                                                                                                     \
} while (0)

#define vec_append(v__, ...) do {                \
    vec_reserve(v__, 1);                         \
    (v__)->items[(v__)->length++] = __VA_ARGS__; \
} while (0)

#define vec_insert(v__, i__, ...) do {                \
    size_t _idx = i__;                                \
    vec_reserve(v__, 1);                              \
    (v__)->items[(v__)->length] = (v__)->items[_idx]; \
    (v__)->items[_idx] = __VA_ARGS__;                 \
    ++(v__)->length;                                  \
} while (0)

#define vec_insert_ordered(v__, i__, ...) do {         \
    size_t _idx = i__;                                 \
    vec_reserve(v__, 1);                               \
    memmove(                                           \
        &(v__)->items[_idx + 1],                       \
        &(v__)->items[_idx],                           \
        ((v__)->length - _idx) * sizeof(*(v__)->items) \
    );                                                 \
    (v__)->items[_idx] = __VA_ARGS__;                  \
    ++(v__)->length;                                   \
} while (0)

#define vec_remove(v__, i__) ((v__)->items[i__] = (v__)->items[--(v__)->length])

#define vec_remove_ordered(v__, i__) do {              \
    size_t _idx = i__;                                 \
    memmove(                                           \
        &(v__)->items[_idx],                           \
        &(v__)->items[_idx + 1],                       \
        ((v__)->length - _idx) * sizeof(*(v__)->items) \
    );                                                 \
    --(v__)->length;                                   \
} while (0)

typedef int (*Vec_CompareFn)(const void*, const void*, size_t);

#ifdef __GNUC__
#define vec_find(v__, it__) ({                                                       \
    __auto_type v = (v__);                                                           \
    __auto_type it = (it__);                                                         \
    vec_find_(v.length, (const void*)v.items, (const void*)&it, sizeof(it), memcmp); \
})
#else
#define vec_find(v__, it__) vec_find_((v__).count, (v__).items, &(it__), sizeof(it__), memcmp)
#endif

int vec_find_(size_t count, const void *items, const void *item, size_t stride, Vec_CompareFn cmp);

#define vec_clear(v__) do { \
    (v__)->length = 0;      \
} while (0)

#define vec_free(v__) do {                                                                \
    sqx_free((v__)->allocator, (v__)->items, (v__)->capacity * sizeof(*(v__)->items));    \
    (v__)->length = 0;                                                                    \
    (v__)->capacity = 0;                                                                  \
    (v__)->items = NULL;                                                                  \
} while (0)

#ifdef __GNUC__
#define vec_foreach(it__, v__) for (    \
    __auto_type it__ = &(v__).items[0]; \
    it__ != &(v__).items[(v__).length]; \
    ++it__                              \
)
#define vec_foreach_rev(it__, v__) for (               \
    __auto_type it__ = &(v__).items[(v__).length - 1]; \
    it__ >= &(v__).items[0];                           \
    --it__                                             \
)
#else
#define vec_foreach(ty__, it__, v__) for ( \
    ty__ it__ = &(v__).items[0];           \
    it__ != &(v__).items[(v__).length];    \
    ++it__                                 \
)
#define vec_foreach_rev(ty__, it__, v__) for (  \
    ty__ it__ = &(v__).items[(v__).length - 1]; \
    it__ >= &(v__).items[0];                    \
    --it__                                      \
)
#endif

#ifdef bool
    #undef bool
    DEFINE_VEC_FOR_TYPE_WITH_NAME(_Bool, bool);
    #define bool _Bool
#else
    DEFINE_VEC_FOR_TYPE_WITH_NAME(_Bool, bool);
#endif

DEFINE_VEC_FOR_TYPE(char);
DEFINE_VEC_FOR_TYPE_WITH_NAME(unsigned char, UChar);
DEFINE_VEC_FOR_TYPE(short);
DEFINE_VEC_FOR_TYPE_WITH_NAME(unsigned short, UShort);
DEFINE_VEC_FOR_TYPE(int);
DEFINE_VEC_FOR_TYPE_WITH_NAME(unsigned int, UInt);
DEFINE_VEC_FOR_TYPE(long);
DEFINE_VEC_FOR_TYPE_WITH_NAME(unsigned long, ULong);
DEFINE_VEC_FOR_TYPE_WITH_NAME(long long, LongLong);
DEFINE_VEC_FOR_TYPE_WITH_NAME(unsigned long long, ULongLong);
DEFINE_VEC_FOR_TYPE(float);
DEFINE_VEC_FOR_TYPE(double);
DEFINE_VEC_FOR_TYPE_WITH_NAME(char *, String);

#endif // _VEC_H_

