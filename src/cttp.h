#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#ifndef ARENA_BACKEND

#if defined(__linux__)
#define ARENA_BACKEND ARENA_BACKEND_LINUX_MMAP
#elif defined(_WIN32)
#define ARENA_BACKEND ARENA_BACKEND_WIN32_VIRTUALALLOC
#endif

#endif // ARENA_BACKEND


#include "../vendored/arena.h"
#include "./base/base_inc.h"

#endif
