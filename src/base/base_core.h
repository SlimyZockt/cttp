#ifndef CTTP_CORE_H
#define CTTP_CORE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i8 b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;
typedef float f32;
typedef double f64;
typedef void VoidProc(void);

#define internal static
#define global static
#define local_persist static
#define rodata static const

#define KB(n)  (((u64)(n)) << 10)
#define MB(n)  (((u64)(n)) << 20)
#define GB(n)  (((u64)(n)) << 30)
#define TB(n)  (((u64)(n)) << 40)
#define Thousand(n)   ((n)*1000)
#define Million(n)    ((n)*1000000)
#define Billion(n)    ((n)*1000000000)
#define static_assert _Static_assert


#define cttp__error(msg, tag)                                   \
    do {                                                        \
        fprintf(stderr,                                         \
            "[%s] --- [%s:%d:%s()] %s \n",                      \
            tag,                                                \
            __FILE__,                                           \
            __LINE__,                                           \
            __func__,                                           \
            msg                                                 \
        );                                                      \
    } while(0)                                                  \

#define cttp_error(msg) cttp__error(msg, "ERROR")

#define cttp__assert(expr, msg, tag)                                    \
    do {                                                                \
        if (!(expr)) {                                                  \
            cttp__error(*(msg) ? (msg) : #expr, tag);                   \
            abort();                                                    \
        }                                                               \
    } while(0)                                                          \


#define cttp_ensure(expr, msg) cttp__assert(expr, msg, "ENSURE FAILED")

#ifndef NDEBUG
    #define cttp_assert(expr, msg) cttp__assert(expr, msg, "ASSERTION FAILED")
#endif

#define cttp_panic(msg) cttp__assert(0, msg, "PANIC")

#define BSWAP16(x) \
    ((((x) & 0x00FF) << 8) | \
     (((x) & 0xFF00) >> 8))

#define BSWAP32(x) \
    ((((x) & 0x000000FF) << 24) | \
     (((x) & 0x0000FF00) << 8)  | \
     (((x) & 0x00FF0000) >> 8)  | \
     (((x) & 0xFF000000) >> 24))

#define BSWAP64(x) \
    ((((x) & 0x00000000000000FFULL) << 56) | \
     (((x) & 0x000000000000FF00ULL) << 40) | \
     (((x) & 0x0000000000FF0000ULL) << 24) | \
     (((x) & 0x00000000FF000000ULL) << 8)  | \
     (((x) & 0x000000FF00000000ULL) >> 8)  | \
     (((x) & 0x0000FF0000000000ULL) >> 24) | \
     (((x) & 0x00FF000000000000ULL) >> 40) | \
     (((x) & 0xFF00000000000000ULL) >> 56))
#endif
