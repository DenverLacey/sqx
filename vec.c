#include "vec.h"

int vec_find_(size_t count, const void *items, const void *item, size_t stride, Vec_CompareFn cmp) {
    char *_items = (char *)items;

    for (size_t i = 0; i < count; ++i) {
        const void *other = &_items[i * stride];
        if (cmp(item, other, stride) == 0) {
            return i;
        }
    }

    return -1;
}

