#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#include "base/base_string.h"
#ifndef ARENA_BACKEND
    #if defined(__linux__)
        #define ARENA_BACKEND ARENA_BACKEND_LINUX_MMAP
    #elif defined(_WIN32)
        #define ARENA_BACKEND ARENA_BACKEND_WIN32_VIRTUALALLOC
    #endif
#endif // ARENA_BACKEND

#include "./base/base_inc.h"

typedef _Array(CTTP_String);

typedef u16 CTTP_StatusCode;
enum {
CTTP_StatusCode_OK = 0, 
CTTP_StatusCode_404 = 1,
};

rodata char *CTTP_StatusCodeTable[] = {
"200 OK\r\n",
"404 Not Found\r\n",
};

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

typedef struct CTTP_Request {
    CTTP_MethodFlag method;
    CTTP_String_Array *path;
} CTTP_Request;

typedef struct CTTP_Response {
    CTTP_StatusCode status_coode;
    CTTP_String body;
    // CTTP_StatusCode type;
} CTTP_Response;

typedef CTTP_Response(*CTTP_Handle)(CTTP_Request*);

typedef struct CTTP_Route {
    CTTP_Handle handle;
    CTTP_MethodFlag method;
    CTTP_String_Array path;
} CTTP_Route;

typedef _Array(CTTP_Route);

typedef struct CTTP_Server_Parameter {
   u64 port;
} CTTP_Server_Parameter;

typedef struct CTTP_Server {
   CTTP_Route_Array *routes;
   int socket;
   u64 port;
   Arena arena;
} CTTP_Server;

#define CTTP_PATH(...) (CTTP_String_Array){                                \
    0,                                                                     \
    (CTTP_String[]){__VA_ARGS__},                                          \
    (sizeof((CTTP_String[]){__VA_ARGS__}) / sizeof(CTTP_String)),          \
    (sizeof((CTTP_String[]){__VA_ARGS__}) / sizeof(CTTP_String)),          \
}

#define cttp_begin(server, ...)                                   \
    cttp_begin_opt(server, &(CTTP_Server_Parameter){.port = 0,    \
                                        __VA_ARGS__})  

void cttp_begin_opt(CTTP_Server *server, CTTP_Server_Parameter *parameter);
void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_String_Array path, CTTP_Handle handle);
void cttp_end(CTTP_Server *server);
#endif
