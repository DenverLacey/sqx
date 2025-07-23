#include "dstring.h"
#include "allocator.h"
#include "keywords.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DString dsdup(DString s) {
    return dscopyn(s.chars, s.length);
}

DString dsdupin(DString s, Allocator allocator) {
    return dscopynin(s.chars, s.length, allocator);
}

DString dscopy(const char *s) {
    size_t n = strlen(s);
    return dscopyn(s, n);
}

DString dscopyin(const char *s, Allocator allocator) {
    size_t n = strlen(s);
    return dscopynin(s, n, allocator);
}

DString dscopyn(const char *s, size_t n) {
    return dscopynin(s, n, (Allocator){0});
}

DString dscopynin(const char *s, size_t n, Allocator allocator) {
    char *chars = sqx_alloc(allocator, n, alignof(char));
    strncpy(chars, s, n);
    chars[n] = 0;
    return (DString) {
        .allocator = allocator,
        .length = n,
        .capacity = n,
        .chars = chars,
    };
}

void dspush(DString *s, char c) {
    dsreserve(s, s->length + 1);
    s->chars[s->length++] = c;
    s->chars[s->length] = '\0';
}

void dsappend(DString *ds, const char *s) {
    size_t n = strlen(s);
    dsappendn(ds, s, n);
}

void dsappendn(DString *ds, const char *s, size_t n) {
    dsreserve(ds, ds->length + n);
    strncat(&ds->chars[ds->length], s, n);
    ds->length += n;
    ds->chars[ds->length] = '\0';
}

void dsextend(DString *ds, DString other) {
    dsappendn(ds, other.chars, other.length);
}

void dsreserve(DString *s, size_t new_cap) {
    if (s->capacity >= new_cap) {
        return;
    }

    size_t ncap = s->capacity == 0 ? 1 : s->capacity;
    do {
        ncap *= DSTRING_GROWTH_FACTOR;
    } while (ncap < new_cap);

    char *new_chars = sqx_realloc(s->allocator, s->chars, (ncap + 1) * sizeof(char), alignof(char));
    if (!new_chars) {
        return;
    }

    s->chars = new_chars;
    s->capacity = ncap;
}

void dsreserve_exact(DString *s, size_t new_cap) {
    if (s->capacity >= new_cap) {
        return;
    }

    char *new_chars = realloc(s->chars, (new_cap + 1) * sizeof(char));
    if (!new_chars) {
        return;
    }

    s->chars = new_chars;
    s->capacity = new_cap;
}

DString dsformat(const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    DString result = vdsformat(fmt, args);
    va_end(args);
    return result;
}

#ifdef _MSC_VER_
    #define vmeasureprint _vscprintf
#elif defined(__GNUC__)
static int vmeasureprint(const char *restrict fmt, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int result = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);
    return result;
}
#endif

DString vdsformat(const char *restrict fmt, va_list args) {
    DString s = {0};
    int nw = vdsprintf(&s, fmt, args);
    if (nw < 0) {
        dsfree(&s);
    }
    return s;
}

int dsprintf(DString *s, const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int nwritten = vdsprintf(s, fmt, args);
    va_end(args);
    return nwritten;
}

int vdsprintf(DString *s, const char *restrict fmt, va_list args) {
    size_t len = vmeasureprint(fmt, args);
    dsreserve(s, len);
    int result = vsnprintf(s->chars, s->capacity + 1, fmt, args);
    s->length += len;
    return result;
}

void dsclear(DString *s) {
    s->length = 0;
}

void dsfree(DString *s) {
    sqx_free(s->allocator, (void*)s->chars, s->capacity);
    s->chars = NULL;
    s->length = 0;
    s->capacity = 0;
}

