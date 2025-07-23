#ifndef _TESTS_H_
#define _TESTS_H_

#include <config.h>
#include <stdio.h>

#define LIST_OF_TESTS \
    X(alloc)          \
    X(array)          \
    X(args)           \
    X(dstring)        \
    X(eflags)         \
    X(errors)         \
    X(hashmap)        \
    X(io)             \
    X(list)           \
    X(logging)        \
    X(sstring)        \
    X(temp)           \
    X(thin_string)    \
    X(vec)

typedef enum TestIdx {
    #define X(test) TestIdx_ ## test,
        LIST_OF_TESTS
    #undef X
    TestIdx_COUNT
} TestIdx;

typedef void(*TestFn)(FILE *out, Config cfg);

extern const TestFn test_fns[TestIdx_COUNT];
extern const char *test_names[TestIdx_COUNT];

#define X(test) void test_ ## test (FILE *out, Config cfg);
    LIST_OF_TESTS
#undef X

TestFn get_test_fn(const char *test);

#endif // _TESTS_H_

