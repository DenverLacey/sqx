#ifndef _BUCKET_ARRAY_H_
#define _BUCKET_ARRAY_H_

#include <stddef.h>

#define BK_BUCKET_IDENT_(nm__, sz__) BucketArrayBucket ## sz__ ## _ ## nm__
#define BK_IDENT_(nm__, sz__) BucketArray ## sz__ ## _ ## nm__

#define DEFINE_BUCKET_ARRAY_FOR_TYPE_WITH_NAME(ty__, sz__, nm__) \
    typedef struct BK_BUCKET_IDENT_(nm__, sz__) {                \
        struct BK_BUCKET_IDENT_(nm__, sz__) *next;               \
        ty__ items[sz__];                                        \
    } BK_BUCKET_IDENT_(nm__, sz__);                              \
    typedef struct BK_IDENT_(nm__, sz__) {                       \
        size_t length;                                           \
        BK_BUCKET_IDENT_(nm__, sz__) *buckets;                   \
    } BK_IDENT_(nm__, sz__)

#define DEFINE_BUCKET_ARRAY_FOR_TYPE(ty__, sz__) DEFINE_BUCKET_ARRAY_FOR_TYPE_WITH_NAME(ty__, sz__, ty__)

DEFINE_BUCKET_ARRAY_FOR_TYPE(int, 32);

#endif // _BUCKET_ARRAY_H_

