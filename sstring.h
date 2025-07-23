#ifndef _SSTRING_H_
#define _SSTRING_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef SSTRING_CAPACITY 
    #define SSTRING_CAPACITY 32
#endif

typedef struct SString {
    char chars[SSTRING_CAPACITY];
} SString;

size_t sslen(const SString *s);
bool ssempty(const SString *s);
SString ssdup(const SString *s);
SString sscopy(const char *s);
SString sscopyn(const char *s, size_t n);
SString ssformat(const char *restrict fmt, ...);
SString vssformat(const char *restrict fmt, va_list args);
int ssprintf(SString *s, const char *restrict fmt, ...);
int vssprintf(SString *s, const char *restrict fmt, va_list args);
void ssclear(SString *s);

#endif // _SSTRING_H_

