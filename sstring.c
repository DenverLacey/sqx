#include "sstring.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

size_t sslen(const SString *s) {
    return strlen(s->chars);
}

bool ssempty(const SString *s) {
    return s->chars[0] == 0;
}

SString ssdup(const SString *s) {
    SString copy;
    strcpy(copy.chars, s->chars);
    return copy;
}

SString sscopy(const char *s) {
    size_t slen = strlen(s);
    return sscopyn(s, slen);
}

SString sscopyn(const char *s, size_t n) {
    SString copy;
    size_t _n = n < SSTRING_CAPACITY ? n : SSTRING_CAPACITY - 1;
    strncpy(copy.chars, s, _n);
    memset(copy.chars + _n, 0, SSTRING_CAPACITY - _n);
    return copy;
}

SString ssformat(const char *__restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SString s = vssformat(fmt, args);
    va_end(args);
    return s;
}

SString vssformat(const char *__restrict fmt, va_list args) {
    SString s;
    vssprintf(&s, fmt, args);
    return s;
}

int ssprintf(SString *s, const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int nw = vssprintf(s, fmt, args);
    va_end(args);
    return nw;
}

int vssprintf(SString *s, const char *restrict fmt, va_list args) {
    return vsnprintf(s->chars, SSTRING_CAPACITY, fmt, args);
}

void ssclear(SString *s) {
    memset(s->chars, 0, SSTRING_CAPACITY);
}

