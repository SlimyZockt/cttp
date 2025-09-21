#ifndef CTTP_BASE_ASSERT_H
#define CTTP_BASE_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define CTTP_RED "\x1B[31m"
#define CTTP_GRN "\x1B[32m"
#define CTTP_YEL "\x1B[33m"
#define CTTP_BLU "\x1B[34m"
#define CTTP_MAG "\x1B[35m"
#define CTTP_CYN "\x1B[36m"
#define CTTP_WHT "\x1B[37m"
#define CTTP_RESET "\x1B[0m"

#define cttp__log(tag, color, msg, ...)                                         \
    fprintf(stderr,                                                             \
        color "[" tag "] --- " CTTP_RESET "[" __FILE__ ":%d:%s()] " msg " \n",  \
        __LINE__,                                                               \
        __func__                                                                \
        ,##__VA_ARGS__)

 
#define cttp__assert(tag, expr, msg, ...)                                    \
    do {                                                                     \
        if (!(expr)) {                                                       \
            cttp__log(tag, CTTP_RED,  msg, ##__VA_ARGS__);                   \
            abort();                                                         \
        }                                                                    \
    } while(0)


#define cttp_ensure(...) cttp__assert("ENSURE FAILED", __VA_ARGS__)

#define cttp_info(...) cttp__log("INFO", CTTP_RESET, __VA_ARGS__)
#define cttp_warn(...) cttp__log("WARN", CTTP_YEL, __VA_ARGS__)
#define cttp_error(...) cttp__log("ERROR", CTTP_RED, __VA_ARGS__)
#define cttp_panic(...) cttp__assert(0, "PANIC", __VA_ARGS__)

#ifndef CTTP_RELEASE
    #define cttp_assert(...) cttp__assert("ASSERTION FAILED", __VA_ARGS__)
    #define cttp_debug(...) cttp__log("DEBUG", CTTP_RESET, __VA_ARGS__)
#endif

#endif
