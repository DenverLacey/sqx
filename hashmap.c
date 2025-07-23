#include "hashmap.h"
#include "allocator.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// TODO:
// - [ ] Add checking keys when hashes equal

typedef uint32_t Hash;

typedef struct Entry {
    Hash hash;
    int32_t index;
} Entry;

struct HashMap_Data {
    Allocator    allocator;
    size_t       value_type_size;
    size_t       value_type_align;
    Entry       *entries;
    const char **keys;
    uint8_t     *values;
};

static Hash MurmurOAAT(const char *k) {
    Hash h = 0x12345678;
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (; *k; ++k) {
        h ^= *k;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

static void recompute_entries(Entry *new, size_t new_cap, Entry *old, size_t old_cap) {
    assert(new_cap > old_cap);

    for (size_t i = 0; i < new_cap; i++) {
        new[i].hash = 0;
        new[i].index = -1;
    }

    for (size_t i = 0; i < old_cap; i++) {
        if (old[i].index == -1) continue;
        size_t j = old[i].hash % new_cap;
        while (new[j].index != -1) {
            j = (j + 1) % new_cap;
        }
        new[j] = old[i];
    }
}

static Entry *get_entry(Entry *entries, size_t sz, Hash hash, bool *pre_existing) {
    *pre_existing = false;
    size_t i = hash % sz;

    while (entries[i].index != -1) {
        if (entries[i].hash == hash) {
            *pre_existing = true;
            break;
        }

        i = (i + 1) % sz;
        if (i == hash % sz) {
            return NULL;
        }
    }

    return &entries[i];
}

static Entry *find_entry(Entry *entries, size_t sz, Hash hash) {
    size_t i = hash % sz;
    while (entries[i].index == -1 || entries[i].hash != hash) {
        i = (i + 1) % sz;
        if (i == hash % sz) {
            return NULL;
        }
    }
    return &entries[i];
}

HashMap hminit(size_t value_type_size, size_t value_type_align) {
    return hminitin(value_type_size, value_type_align, (Allocator){0});
}

HashMap hminitin(size_t value_type_size, size_t value_type_align, Allocator allocator) {
    HashMap_Data *data = sqx_calloc(allocator, sizeof(HashMap_Data), alignof(HashMap_Data));
    data->value_type_size = value_type_size;
    data->value_type_align = value_type_align;
    data->allocator = allocator;
    return (HashMap){.data = data};
}

void hmfree(HashMap *m) {
    sqx_free(m->data->allocator, m->data->entries, m->capacity * sizeof(Entry));
    sqx_free(m->data->allocator, m->data->values, m->capacity * m->data->value_type_size);
    for (size_t i = 0; i < m->length; i++) sqx_free(m->data->allocator, m->data->keys[i], strlen(m->data->keys[i]));
    sqx_free(m->data->allocator, m->data->keys, m->capacity * sizeof(char*));
    sqx_free(m->data->allocator, m->data, sizeof(HashMap_Data));
    memset(m, 0, sizeof(*m));
}

// FIXME: Call to hmreserve even if key is pre-existing
void hmputp(HashMap *m, const char *key, const void *value) {
    hmreserve(m, 1);

    Hash h = MurmurOAAT(key);

    bool pre_existing;
    Entry *e = get_entry(m->data->entries, m->capacity, h, &pre_existing);

    if (!pre_existing) {
        e->hash = h;
        e->index = m->length;

        const char *new_key = strdup(key);
        m->data->keys[e->index] = new_key;
    }

    void *value_ptr = &m->data->values[e->index * m->data->value_type_size];
    memcpy(value_ptr, value, m->data->value_type_size);

    m->length++;
}

void *hmget(HashMap m, const char *key) {
    Hash h = MurmurOAAT(key);
    Entry *e = find_entry(m.data->entries, m.capacity, h);
    if (!e) return NULL;
    return &m.data->values[e->index * m.data->value_type_size];
}

bool hmremove(HashMap *m, const char *key) {
    Hash h = MurmurOAAT(key);
    Entry *e = find_entry(m->data->entries, m->capacity, h);
    if (!e) return false;

    Hash h_last = MurmurOAAT(m->data->keys[m->length - 1]); 
    Entry *last = find_entry(m->data->entries, m->capacity, h_last);
    assert(last);

    m->data->keys[e->index] = m->data->keys[last->index];
    memcpy(&m->data->values[e->index], &m->data->values[last->index * m->data->value_type_size], m->data->value_type_size);

    last->index = e->index;
    e->hash = 0;
    e->index = -1;

    m->length--;

    return true;
}

const char **hmkeys(HashMap m) {
    return m.data->keys;
}

void *hmvalues(HashMap m) {
    return m.data->values;
}

bool hmcontains(HashMap m, const char *key) {
    Hash h = MurmurOAAT(key);
    size_t i = h % m.capacity;
    do {
        if (m.data->entries[i].index != -1 && m.data->entries[i].hash == h) {
            return true;
        }
        i = (i + 1) % m.capacity;
    } while (i != h % m.capacity);
    return false;
}

void hmreserve(HashMap *m, size_t n) {
    size_t new_size = m->length + n;
    if (new_size > (m->capacity * HASHMAP_RESIZE_THRESHOLD)) {
        size_t new_cap = m->capacity != 0 ? m->capacity : 16;
        while (new_cap < new_size) new_cap *= HASHMAP_GROWTH_FACTOR;

        Entry *new_entries = sqx_alloc(m->data->allocator, new_cap * sizeof(*new_entries), alignof(Entry));
        recompute_entries(new_entries, new_cap, m->data->entries, m->capacity);
        sqx_free(m->data->allocator, m->data->entries, m->capacity * sizeof(Entry));
        m->data->entries = new_entries;

        m->data->keys   = sqx_realloc(m->data->allocator, m->data->keys, new_cap * sizeof(*m->data->keys), alignof(char*));
        m->data->values = sqx_realloc(m->data->allocator, m->data->values, new_cap * m->data->value_type_size, m->data->value_type_align);

        m->capacity = new_cap;
    }
}

void hmreserve_exact(HashMap *m, size_t n) {
    size_t new_size = m->length + n;
    if (new_size > (m->capacity * HASHMAP_RESIZE_THRESHOLD)) {
        Entry *new_entries = sqx_alloc(m->data->allocator, new_size * sizeof(*new_entries), alignof(Entry));
        recompute_entries(new_entries, new_size, m->data->entries, m->capacity);
        sqx_free(m->data->allocator, m->data->entries, m->capacity * sizeof(Entry));
        m->data->entries = new_entries;

        m->data->keys   = sqx_realloc(m->data->allocator, m->data->keys, new_size * sizeof(*m->data->keys), alignof(char*));
        m->data->values = sqx_realloc(m->data->allocator, m->data->values, new_size * m->data->value_type_size, m->data->value_type_align);

        m->capacity = new_size;
    }
}

HashMapIter hmiter(HashMap m) {
    return (HashMapIter) {
        .length          = m.length,
        .value_type_size = m.data->value_type_size,
        .keys            = m.data->keys,
        .values          = (void*)m.data->values,
    };
}

bool hmnext(HashMapIter *iter, const char **key, void **value) {
    const char *keyp = NULL;
    void *valuep = NULL;
    bool continue_ = true;

    if (iter->length != 0) {
        iter->length--;
        keyp = *iter->keys++;
        valuep = iter->values;
        iter->values = (char*)iter->values + iter->value_type_size;
    } else {
        continue_ = false;
    }

    if (key)   *key   = keyp;
    if (value) *value = valuep;
    return continue_;
}

