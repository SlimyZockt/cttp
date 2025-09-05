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



typedef struct CTTP_String {
    char *str;
    u64 len;
} CTTP_String;

typedef struct CTTP_ServerParam {
   u64 port;
} CTTP_ServerParam;

typedef struct CTTP_Path{
    const char **path;
    size_t count;
    // size_t capacity;
} CTTP_Path;

typedef struct CTTP_Respose {

} CTTP_Respose;

typedef struct CTTP_Request {

} CTTP_Request;

typedef b8(*CTTP_Handle)(CTTP_Respose, CTTP_Request);

typedef struct CTTP_Route {
    CTTP_Path path; 
    CTTP_Handle handle;
} CTTP_Route;

typedef struct CTTP_Server {
   int socket;
   CTTP_Route *routes;
   u64 route_count;
   Arena arena;
} CTTP_Server;

typedef u16 CTTP_MethodFlag;
enum {
CTTP_MethodFlag_GET = (1 << 0),
CTTP_MethodFlag_HEAD = (1 << 1),
CTTP_MethodFlag_POST = (1 << 2),
CTTP_MethodFlag_PUT = (1 << 3),
CTTP_MethodFlag_DELETE = (1 << 4),
CTTP_MethodFlag_CONNECT = (1 << 5),
CTTP_MethodFlag_OPTIONS = (1 << 6),
CTTP_MethodFlag_TRACE = (1 << 7),
CTTP_MethodFlag_PATCH = (1 << 8),
};

#define cttp_path(...)                                               \
(CTTP_Path){((const char *[]){__VA_ARGS__}),                         \
        (sizeof((const char *[]){__VA_ARGS__}) / sizeof(const char *))}

#define cttp_start(...)                                 \
    cttp_start_opt(&(CTTP_ServerParam){.port = 8080,    \
                                        __VA_ARGS__})   



CTTP_Server cttp_start_opt(CTTP_ServerParam *param);
void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_Path path, CTTP_Handle handle);
void cttp_end(CTTP_Server *server);
#endif
