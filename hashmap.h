#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "allocator.h"
#include "keywords.h"
#include <stddef.h>

#define HASHMAP_GROWTH_FACTOR 2
#define HASHMAP_RESIZE_THRESHOLD 0.75f

typedef struct HashMap_Data HashMap_Data;

typedef struct HashMap {
    size_t length, capacity;
    HashMap_Data *data;
} HashMap;

#define hmnew(type__) hminit(sizeof(type__), alignof(type__))
HashMap hminit(size_t value_type_size, size_t value_type_align);
#define hmnewin(type__, allocator__) hminitin(sizeof(type__), alignof(type__), allocator__)
HashMap hminitin(size_t value_type_size, size_t value_type_align, Allocator allocator);
void hmfree(HashMap *m);

#ifdef __GNUC__
#define hmput(m__, key__, ...) do {          \
    const char *key = (key__);               \
    __auto_type value = (__VA_ARGS__);       \
    hmputp((m__), key, (const void*)&value); \
} while (0)
#endif

void hmputp(HashMap *m, const char *key, const void *value);
void *hmget(HashMap m, const char *key);
bool hmremove(HashMap *m, const char *key);

const char **hmkeys(HashMap m);
void *hmvalues(HashMap m);

bool hmcontains(HashMap m, const char *key);

void hmreserve(HashMap *m, size_t n);
void hmreserve_exact(HashMap *m, size_t n);

typedef struct HashMapIter {
    size_t       length;
    size_t       value_type_size;
    const char **keys;
    void        *values;
} HashMapIter;

HashMapIter hmiter(HashMap m);
bool hmnext(HashMapIter *iter, const char **key, void **value);

#endif // _HASHMAP_H_

