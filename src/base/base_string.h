#ifndef CTTP_BASE_STRING_H
#define CTTP_BASE_STRING_H

#include "base_core.h"
#include <stddef.h>

typedef struct CTTP_String {
    char* str;
    u64 len;
} CTTP_String;

#define CTTP_S(str) (CTTP_String){str, sizeof(str) - 1}

typedef struct CTTP_String_Builder {
    char *items;
    size_t count;
    size_t capacity;
} CTTP_String_Builder;

internal 
b8 cttp_char_is_space(u8 c) {
  return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v');
}

internal 
b8 cttp_str_cmp(CTTP_String target, CTTP_String cmp) {
    if (target.len != cmp.len) return 0;

    for (u64 i = 0; i < target.len; i++) {
        if (target.str[i] != cmp.str[i]) return 0;
    }

    return 1;
}

#define cttp_str_cmp_many(target, ...) cttp_str_cmp_many_(target, (CTTP_String[]){__VA_ARGS__}, (sizeof((CTTP_String[]){__VA_ARGS__}) / sizeof(CTTP_String)))

internal 
b8 cttp_str_cmp_many_(CTTP_String target, CTTP_String strings[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        if ( !cttp_str_cmp(target, strings[i]) ) return 0;
    }

    return 1;
}
#endif
