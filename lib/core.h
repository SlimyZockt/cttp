#include <stddef.h>
#include <stdint.h>

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

typedef struct {
    u8 *memory;
    size_t capacity;
    size_t offset;
} Arena;

Arena create_arena(size_t size);
