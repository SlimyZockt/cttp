#ifndef BASE_ARENA_H
#define BASE_ARENA_H
#define BASE_ARENA_PREFIX
// #include "./base_core.h"

typedef u8 ArenaFlags;
enum {
    ArenaFlag_Growing = (1 << 0),
    // ArenaFlag_LargePages = (1 << 1),
};

typedef struct ArenaParams {
    ArenaFlags flags;
    // u64 reserve_size;
    // u64 commit_size;
    // void *optional_backing_buffer;
    char *allocation_site_file;
    int allocation_site_line;
} ArenaParams;


typedef struct ArenaBlock {
    u64 capacity;
    u64 offset;
} ArenaBlock

typedef struct Arena {
    Arena *prev;
    Arena *current;
    ArenaFlags flags;
    u64 capacity;
    u64 offset;
    char *allocation_site_file;
    int allocation_site_line;
} Arena;



global u64 arena_default_reserve_size = MB(64);
global u64 arena_default_commit_size = KB(64);
global ArenaFlags arena_default_flags = ArenaFlag_Growing;


BASE_ARENA_PREFIX *arena_alloc_(ArenaParams *params);
#define arena_alloc(...)                                                       \
    arena_alloc_(&(ArenaParams){.reserve_size = arena_default_reserve_size,    \
                                .commit_size = arena_default_commit_size,      \
                                .flags = arena_default_flags,                  \
                                .allocation_site_file = __FILE__,              \
                                .allocation_site_line = __LINE__,              \
                                __VA_ARGS__})
BASE_ARENA_PREFIX void arena_release(Arena *arena);

BASE_ARENA_PREFIX void *arena_push(Arena *arena, U64 size, U64 align);
BASE_ARENA_PREFIX U64 arena_pos(Arena *arena);
BASE_ARENA_PREFIX void arena_pop_to(Arena *arena, U64 pos);

//- rjf: arena push/pop helpers
BASE_ARENA_PREFIX void arena_clear(Arena *arena);
BASE_ARENA_PREFIX void arena_pop(Arena *arena, U64 amt);

//- rjf: temporary arena scopes
BASE_ARENA_PREFIX Temp temp_begin(Arena *arena);
BASE_ARENA_PREFIX void temp_end(Temp temp);

//- rjf: push helper macros
#define push_array_no_zero_aligned(a, T, c, align)                             \
    (T *)arena_push((a), sizeof(T) * (c), (align))
#define push_array_aligned(a, T, c, align)                                     \
    (T *)MemoryZero(push_array_no_zero_aligned(a, T, c, align), sizeof(T) * (c))
#define push_array_no_zero(a, T, c)                                            \
    push_array_no_zero_aligned(a, T, c, Max(8, AlignOf(T)))
#define push_array(a, T, c) push_array_aligned(a, T, c, Max(8, AlignOf(T)))

#ifdef BASE_ARENA_IMPLEMENTATION
BASE_ARENA_PREFIX Arena *arena_alloc_(ArenaParams *params) {

   void *mem = mmap(NULL, params->, int prot, int flags,
           int fildes, off_t off);


};
#endif

#endif
