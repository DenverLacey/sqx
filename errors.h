#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdarg.h>
#include <stdbool.h>

#include "attributes.h"
#include "macro_ops.h"

#define throws

#define throw \
    goto catch

#define fail(...) do {     \
    seterror(__VA_ARGS__); \
    throw;                 \
} while (0)

#define vfail(...) do {     \
    vseterror(__VA_ARGS__); \
    throw;                  \
} while (0)

#ifdef __GNUC__
#define try(...) ({                                                      \
    reseterror();                                                        \
    UNUSED static __typeof__((__VA_ARGS__)) *GENSYM(try_dummy_ln);       \
    __auto_type GENSYM(try_result_ln) = _Generic((GENSYM(try_dummy_ln)), \
        void*: ((__VA_ARGS__), 0),                                       \
        default: (__VA_ARGS__));                                         \
    if (erred()) {                                                       \
        throw;                                                           \
    }                                                                    \
    GENSYM(try_result_ln);                                               \
})
#else
#define try(...) do { \
    reseterror();     \
    __VA_ARGS__;      \
    if (erred()) {    \
        throw;        \
    }                 \
} while (0)
#endif

#define ensure(cond__, ...) do { \
    reseterror();                \
    if (!(cond__)) {             \
        fail(__VA_ARGS__);       \
    }                            \
} while (0)

#define vensure(cond__, ...) do { \
    reseterror();                 \
    if (!(cond__)) {              \
        vfail(__VA_ARGS__);       \
    }                             \
} while (0)

#define erreturn(...) do {                                               \
    result = (__VA_ARGS__);                                              \
    if (!erred()) seterror("error raised at %s:%d", __FILE__, __LINE__); \
    throw;                                                               \
} while (0)

void reseterror(void);
void seterror(const char *restrict fmt, ...);
void vseterror(const char *restrict fmt, va_list args);
bool erred(void);
const char *geterror(void);

#endif // _ERRORS_H_

