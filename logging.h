#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdlib.h>
#include <stdio.h>
#include "macro_ops.h"

#define HERE __FILE__ ":" STRINGIFY(__LINE__)

#ifdef __GNUC__
#define FLOG(f__, lv__, ...) ({       \
    FILE *f = (f__);                  \
    const char *lv = (lv__);          \
    fprintf(f, "%s: %s: ", HERE, lv); \
    fprintf(f, __VA_ARGS__);          \
    fprintf(f, "\n");                 \
})
#else
#define FLOG(f__, lv__, ...) (                   \
    fprintf((f__), "%s", HERE) +                 \
    fprintf((f__), ": " lv__ ": " __VA_ARGS__) + \
    fprintf((f__), "\n")                         \
)
#endif

#define LOG(lv__, ...) FLOG(stdout, lv__, __VA_ARGS__)

#define FINFO(f__, ...) FLOG(f__, "INFO", __VA_ARGS__)
#define INFO(...) FINFO(stdout, __VA_ARGS__)

#define FDBG(f__, ...) FLOG(f__, "DEBUG", __VA_ARGS__)
#define DBG(...) FDBG(stderr, __VA_ARGS__)

#define FERROR(f__, ...) FLOG(f__, "ERROR", __VA_ARGS__)
#define ERROR(...) FERROR(stderr, __VA_ARGS__)

#define TODO(...) \
    (fprintf(stderr, "%s", HERE), fprintf(stderr, ": TODO: " __VA_ARGS__), fprintf(stderr, "\n"), exit(EXIT_FAILURE))

#define UNIMPLEMENTED \
    TODO("%s has not been implemented.", __func__)

#endif // _LOGGING_H_

