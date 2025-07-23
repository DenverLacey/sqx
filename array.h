#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "logging.h"
#include "macro_ops.h"
#include <stddef.h>

#define ARRAY_IDENT_(nm__) CONCAT(Array_, nm__)

#define DEFINE_ARRAY_FOR_TYPE_WITH_NAME(ty__, nm__) \
    typedef struct ARRAY_IDENT_(nm__) {             \
        size_t length;                              \
        ty__ *items;                                \
    } ARRAY_IDENT_(nm__)

#define DEFINE_ARRAY_FOR_TYPE(ty__) DEFINE_ARRAY_FOR_TYPE_WITH_NAME(ty__, ty__)

#ifdef __GNUC__
#define arr_of(...) \
    {.length = COUNT_VARARGS(__VA_ARGS__), .items = (__typeof__(FIRST_VARARGS(__VA_ARGS__))[]){ __VA_ARGS__ }}
#else
#define arr_of(ty__, ...) {.length = COUNT_VARARGS(__VA_ARGS__), .items = (ty__[]){ __VA_ARGS__ }}
#endif

typedef int (*Array_CompareFn)(const void*, const void*, size_t);

#ifdef __GNUC__
#define arr_find(a__, it__) ({                                                       \
    __auto_type a = (a__);                                                           \
    __auto_type it = (it__);                                                         \
    arr_find_(a.length, (const void*)a.items, (const void*)&it, sizeof(it), memcmp); \
})
#else
#define arr_find(a__, it__) arr_find_((a__).length, (a__).items, &(it__), sizeof(it__), memcmp)
#endif

int arr_find_(size_t count, const void *items, const void *item, size_t stride, Array_CompareFn cmp);

#ifdef __GNUC__
#define arr_lst(a__) ({                          \
    __auto_type a = (a__);                       \
    if (a.length == 0) {                         \
        ERROR("Attempt to access empty array."); \
        exit(1);                                 \
    }                                            \
    a.items[a.length-1];                         \
})
#else
#define arr_lst(a__) ((a__).length == 0 ? (ERROR("Attempt to access empty."), exit(1)) : (a__).items[(a__).length-1])
#endif

#ifdef __GNUC__
#define arr_foreach(it__, a__) for (    \
    __auto_type it__ = &(a__).items[0]; \
    it__ != &(a__).items[(a__).length]; \
    ++it__                              \
)
#define arr_foreach_rev(it__, a__) for (               \
    __auto_type it__ = &(a__).items[(a__).length - 1]; \
    it__ >= &(a__).items[0];                           \
    --it__                                             \
)
#else
#define arr_foreach(ty__, it__, a__) for ( \
    ty__ it__ = &(a__).items[0];           \
    it__ != &(a__).items[(a__).length];    \
    ++it__                                 \
)
#define arr_foreach_rev(ty__, it__, a__) for (  \
    ty__ it__ = &(a__).items[(a__).length - 1]; \
    it__ >= &(a__).items[0];                    \
    --it__                                      \
)
#endif

#ifdef bool
    #undef bool
    DEFINE_ARRAY_FOR_TYPE_WITH_NAME(_Bool, bool);
    #define bool _Bool
#else
    DEFINE_ARRAY_FOR_TYPE_WITH_NAME(_Bool, bool);
#endif

DEFINE_ARRAY_FOR_TYPE(char);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(unsigned char, UChar);
DEFINE_ARRAY_FOR_TYPE(short);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(unsigned short, UShort);
DEFINE_ARRAY_FOR_TYPE(int);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(unsigned int, UInt);
DEFINE_ARRAY_FOR_TYPE(long);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(unsigned long, ULong);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(long long, LongLong);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(unsigned long long, ULongLong);
DEFINE_ARRAY_FOR_TYPE(float);
DEFINE_ARRAY_FOR_TYPE(double);
DEFINE_ARRAY_FOR_TYPE_WITH_NAME(char *, String);

#endif // _ARRAY_H_

