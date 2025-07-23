#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#ifndef __GNUC__
#error "[SQX] This Linked List implementation only works with GNU extensions"
#endif

#include "allocator.h"
#include "asserts.h"
#include "keywords.h"
#include "macro_ops.h"

#include <stddef.h>

#define LIST_NODE_IDENT_(ty__) CONCAT(ListNode_, ty__)
#define LIST_IDENT_(ty__) CONCAT(List_, ty__)

#define DEFINE_LIST_FOR_TYPE_WITH_NAME(ty__, nm__) \
    typedef struct LIST_NODE_IDENT_(nm__) {        \
        struct LIST_NODE_IDENT_(nm__) *next;       \
        ty__ item;                                 \
    } LIST_NODE_IDENT_(nm__);                      \
    typedef struct LIST_IDENT_(nm__) {             \
        Allocator allocator;                       \
        size_t length;                             \
        LIST_NODE_IDENT_(nm__) *first;             \
    } LIST_IDENT_(nm__)

#define list_prepend(ls__, it__) ({                                                                                                   \
    __auto_type ls = (ls__);                                                                                                          \
    __typeof__(ls->first) node = (__typeof__(ls->first))sqx_alloc(ls->allocator, sizeof(*ls->first), alignof(__typeof__(ls->first))); \
    node->next = ls->first;                                                                                                           \
    node->item = (it__);                                                                                                              \
    ls->first = node;                                                                                                                 \
    ls->length++;                                                                                                                     \
    node;                                                                                                                             \
})

#define list_free(ls__) do {                          \
    __auto_type ls = (ls__);                          \
    __auto_type node = ls->first;                     \
    while (node) {                                    \
        __auto_type next = node->next;                \
        sqx_free(ls->allocator, node, sizeof(*node)); \
        node = next;                                  \
    }                                                 \
    ls->length = 0;                                   \
    ls->first = NULL;                                 \
} while (0)

#define list_iter(nd__, it__) ({ \
    bool result = false;         \
    __auto_type nd = (nd__);     \
    if (*nd) {                   \
        *(it__) = &(*nd)->item;  \
        *nd = (*nd)->next;       \
        result = true;           \
    }                            \
    result;                      \
})

#define list_get(ls__, idx__) list_get_node_(ls__, idx__)->item

#define list_remove(ls__, idx__) do {             \
    __auto_type ls = (ls__);                      \
    size_t idx = (size_t)(idx__);                 \
    if (idx == 0) {                               \
        ls->first = ls->first->next;              \
        return;                                   \
    }                                             \
    ASSERT_LT(idx, ls->length);                   \
    __auto_type node = list_get_node_(ls, idx-1); \
    node->next = node->next->next;                \
} while (0)

#define list_get_node_(ls__, idx__) ({            \
    __auto_type ls = (ls__);                      \
    size_t idx = (size_t)(idx__);                 \
    ASSERT_GE(idx, 0);                            \
    ASSERT_LT(idx, ls.length);                    \
    __auto_type n = ls.first;                     \
    for (size_t i = 0; i < idx; i++) n = n->next; \
    n;                                            \
})

#define DEFINE_LIST_FOR_TYPE(ty__) DEFINE_LIST_FOR_TYPE_WITH_NAME(ty__, ty__)

#ifdef bool
    #undef bool
    DEFINE_LIST_FOR_TYPE_WITH_NAME(_Bool, bool);
    #define bool _Bool
#else
    DEFINE_LIST_FOR_TYPE_WITH_NAME(_Bool, bool);
#endif

DEFINE_LIST_FOR_TYPE(char);
DEFINE_LIST_FOR_TYPE_WITH_NAME(unsigned char, UChar);
DEFINE_LIST_FOR_TYPE(short);
DEFINE_LIST_FOR_TYPE_WITH_NAME(unsigned short, UShort);
DEFINE_LIST_FOR_TYPE(int);
DEFINE_LIST_FOR_TYPE_WITH_NAME(unsigned int, UInt);
DEFINE_LIST_FOR_TYPE(long);
DEFINE_LIST_FOR_TYPE_WITH_NAME(unsigned long, ULong);
DEFINE_LIST_FOR_TYPE_WITH_NAME(long long, LongLong);
DEFINE_LIST_FOR_TYPE_WITH_NAME(unsigned long long, ULongLong);
DEFINE_LIST_FOR_TYPE(float);
DEFINE_LIST_FOR_TYPE(double);
DEFINE_LIST_FOR_TYPE_WITH_NAME(char *, String);

#endif // _LINKED_LIST_H_

