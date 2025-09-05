#ifndef CTTP_CORE_H
#define CTTP_CORE_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s8 b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;
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

#define StaticAssert _Static_assert

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
