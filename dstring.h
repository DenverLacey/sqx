#ifndef _DSTRING_H_
#define _DSTRING_H_

#include "allocator.h"
#include <stdarg.h>
#include <stddef.h>

#ifndef DSTRING_GROWTH_FACTOR 
    #define DSTRING_GROWTH_FACTOR (2)
#endif

typedef struct DString {
    Allocator allocator;
    size_t length, capacity;
    char *chars;
} DString;

#define dsinit(allocator__) (DString){.allocator=(allocator__)}

DString dsdup(DString s);
DString dsdupin(DString s, Allocator allocator);
DString dscopy(const char *s);
DString dscopyin(const char *s, Allocator allocator);
DString dscopyn(const char *s, size_t n);
DString dscopynin(const char *s, size_t n, Allocator allocator);

void dspush(DString *s, char c);
void dsappend(DString *ds, const char *s);
void dsappendn(DString *ds, const char *s, size_t n);
void dsextend(DString *ds, DString other);

void dsreserve(DString *s, size_t new_cap);
void dsreserve_exact(DString *s, size_t new_cap);

DString dsformat(const char *restrict fmt, ...);
DString vdsformat(const char *restrict fmt, va_list args);

int dsprintf(DString *s, const char *restrict fmt, ...);
int vdsprintf(DString *s, const char *restrict fmt, va_list args);

void dsclear(DString *s);
void dsfree(DString *s);

#endif // _DSTRING_H_

