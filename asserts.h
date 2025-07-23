#ifndef _ASSERTS_H_
#define _ASSERTS_H_

#include <stdio.h>
#include <stdlib.h>
#include "logging.h"

#define FORMAT_SPECIFIER_OF_(...) _Generic((__VA_ARGS__), \
    _Bool: "%s",                                          \
    char: "%c",                                           \
    int: "%d",                                            \
    unsigned: "%d",                                       \
    size_t: "%zu",                                        \
    float: "%f",                                          \
    default: "%s"                                         \
)

#define PRINTF_ARG_(...) _Generic((__VA_ARGS__), \
    _Bool: ((__VA_ARGS__) ? "true" : "false"),   \
    default: (__VA_ARGS__)                       \
)

#define PRINT_VALUE_(...) \
    (printf(FORMAT_SPECIFIER_OF_(__VA_ARGS__), PRINTF_ARG_(__VA_ARGS__)))

#define ASSERT(cond__, ...) do {                                              \
    if (!(cond__)) {                                                          \
        printf("%s:%d ASSERT (%s): ", __FILE__, __LINE__, STRINGIFY(cond__)); \
        printf("" __VA_ARGS__);                                               \
        printf("\n");                                                         \
        exit(EXIT_FAILURE);                                                   \
    }                                                                         \
} while (0)

#define ASSERT_CMP(x__, y__, op__) do {                                             \
    if (!((x__) op__ (y__))) {                                                      \
        printf("%s:%d ASSERT (%s): ", __FILE__, __LINE__, STRINGIFY(x__ op__ y__)); \
        printf("%s:%d ASSERT: %s = ", __FILE__, __LINE__, STRINGIFY(x__));          \
        PRINT_VALUE_(x__);                                                          \
        printf("\n%s:%d ASSERT: %s = ", __FILE__, __LINE__, STRINGIFY(y__));        \
        PRINT_VALUE_(y__);                                                          \
        printf("\n");                                                               \
        exit(EXIT_FAILURE);                                                         \
    }                                                                               \
} while (0)

#define ASSERT_EQ(x__, y__) ASSERT_CMP(x__, y__, ==)
#define ASSERT_NE(x__, y__) ASSERT_CMP(x__, y__, !=)
#define ASSERT_LT(x__, y__) ASSERT_CMP(x__, y__, <)
#define ASSERT_GT(x__, y__) ASSERT_CMP(x__, y__, >)
#define ASSERT_LE(x__, y__) ASSERT_CMP(x__, y__, <=)
#define ASSERT_GE(x__, y__) ASSERT_CMP(x__, y__, >=)

#endif // _ASSERTS_H_

