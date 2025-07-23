#ifndef _EFLAGS_H_
#define _EFLAGS_H_

#define EFlags(type__) int

#define efset(ef__, fl__)     (*(ef__) |=  (1 << (fl__)))
#define efclear(ef__, fl__)   (*(ef__) &= ~(1 << (fl__)))
#define eftoggle(ef__, fl__)  (*(ef__) ^=  (1 << (fl__)))
#define efcheck(ef__, fl__) (((ef__) & (1 << (fl__))) == (1 << (fl__)))
#define efcount(ef__) (EF_POPCOUNT_(ef__))

#ifndef SQX_NO_POPCOUNT
    #ifdef _WIN32
        #include <intrin.h>
        #define EF_POPCOUNT_(...) __popcnt(__VA_ARGS__)
    #elif __GNUC__
        #define EF_POPCOUNT_(...) __builtin_popcount(__VA_ARGS__)
    #else
        #define EF_POPCOUNT_(...) sqx_slow_popcount_(__VA_ARGS__)
    #endif
#else
    int sqx_slow_popcount_(int x);
    #define EF_POPCOUNT_(...) sqx_slow_popcount_(__VA_ARGS__)
#endif

#endif // _EFLAGS_H_

