#ifndef _UNREACHABLE_H_
#define _UNREACHABLE_H_

#ifdef __GNUC__ // GCC, Clang, ICC
    #define UNREACHABLE (__builtin_unreachable())
#elif defined(_MSC_VER) // MSVC
    #define UNREACHABLE (__assume(0))
#else
    _Noreturn inline void unreachable_impl();
    #define UNREACHABLE (unreachable_impl())
#endif

#endif // _UNREACHABLE_H_

