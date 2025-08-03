#include "eflags.h"

#include "attributes.h"

#ifdef SQX_NO_POPCOUNT
int sqx_slow_popcount_(int x) {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    return (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
#else
UNUSED static int sqx_slow_popcount_(UNUSED int x) {return 0;}
#endif

