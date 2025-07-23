#ifdef SQX_FEATURE_DEFER

#ifndef _DEFER_H_
#define _DEFER_H_

#include "macro_ops.h"

#if !defined(__GNUC__) || defined(__clang__)
#error "[SQX] Defer features only work with GCC compiler. Please change compiler or undefine `SQX_FEATURE_DEFER`."
#endif

#define defer                                                               \
    auto void GENSYM(defer_func)(int*);                                     \
    int GENSYM(defer_var) __attribute__((__cleanup__(GENSYM(defer_func)))); \
    auto void GENSYM(defer_func)(int*)

#endif // _DEFER_H_

#endif // SQX_FEATURE

