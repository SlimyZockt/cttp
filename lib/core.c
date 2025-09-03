#include "./core.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

Arena create_arena(size_t capacity) {
    void *mem = mmap(NULL, capacity, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        printf("Arena create failed");
        exit(0);
    }

    return (Arena){
        .memory = mem,
        .capacity = capacity,
        .offset = 0,
    };
}
