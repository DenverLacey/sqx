#ifndef _KEYWORDS_H_
#define _KEYWORDS_H_

#if __STDC_VERSION__ < 202311L
    #ifndef alignof
        #if __STDC_VERSION__ >= 201112L
            #define alignof _Alignof
        #endif
    #endif
    #ifndef bool
        #if __STDC_VERSION__ >= 199901L
            #define bool _Bool
        #endif
    #endif
    #ifndef true
        #if __STDC_VERSION__ >= 199901L
            #define true ((bool)1)
        #endif
    #endif
    #ifndef false
        #if __STDC_VERSION__ >= 199901L
            #define false ((bool)0)
        #endif
    #endif
    #ifndef thread_local
        #if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
            #define thread_local _Thread_local
        #elif defined(_WIN32) && (defined(_MSC_VER) || defined(__ICL) || defined(__DMC__) || defined(__BORLANDC__))
            #define thread_local declspec(thread)
        #elif defined(__GNUC__) || defined(__SINPRO_C) || defined(__hpux) || defined(__xlC__)
            #define thread_local __thread
        #endif
    #endif
#endif

#endif // _KEYWORDS_H_

