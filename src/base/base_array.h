#ifndef CTTP_BASE_ARRAY_H
#define CTTP_BASE_ARRAY_H

// #include "../../vendored/arena.h"

#ifndef ARRAY_INIT_CAPACITY
    #define ARRAY_INIT_CAPACITY 32
#endif


#define array_init(Type, arena) (Type##_Array *) array_init_(sizeof(Type), ARRAY_INIT_CAPACITY, arena)

#define _Array(Type) \
struct Type##_Array { \
    b8 is_dynamic; \
    Type *data; \
    size_t length; \
    size_t capacity; \
} Type##_Array 

typedef struct Array_Header {
    size_t _item_size;
    size_t _capacity_grow;
    Arena *arena;
} Array_Header;

typedef _Array(void);

internal
void_Array *array_init_(size_t item_size, size_t capacity, Arena *arena) {
    void_Array *array = arena_alloc(arena, sizeof(void_Array) + sizeof(Array_Header));
    Array_Header *header = (Array_Header *)((char *)array + sizeof(void_Array));
    if (array == NULL) {
        cttp_assert(0, "Array Header allocation failed");
    }

    array->data = arena_alloc(arena, capacity * item_size);
    if (array->data == NULL) {
        cttp_assert(0, "Array allocation failed");
    }
    array->is_dynamic = 1;
    array->length = 0;
    array->capacity = capacity;

    header->arena = arena;
    header->_capacity_grow = capacity;
    header->_item_size = item_size;

    return array;
}

// #define array_push(array, obj) array_push_((void_Array *)array, obj)
#define array_push(array, item) array_push_((void_Array *)array, (void *)(item));

//TODO: Replace with a more efficient method
#define array_push_many(array, new_items, new_items_count) \
for (u64 i = 0; i < new_items_count; i++) {                \
    array_push(array, new_items[i]);                    \
}

internal
void array_push_(void_Array *array, void *item) {
    cttp_assert(item != NULL, "Tried to append NULL");
    cttp_assert(array->is_dynamic, "Array is not dynamic");
    Array_Header *header = (Array_Header *)((char *)array + sizeof(void_Array));

    cttp_assert(header != NULL, "");
    cttp_assert(sizeof(*item) != header->_item_size, "Item has a different size");

    if (array->length >= array->capacity) {
        array->data = arena_realloc(
            header->arena,
            array->data,
            array->capacity * header->_item_size,
            (array->capacity + header->_capacity_grow) * header->_item_size
        );
        array->capacity += header->_capacity_grow;
    }

    arena_memcpy(
        (array->data + (array->length * header->_item_size)),
        item,
        header->_item_size
    );

    array->length += 1;
}


#define array_pop(array) array_pop_((void_Array *)array)

internal
void array_pop_(void_Array *array) {
    cttp_assert(array->length == 0, "No Item Left");
    cttp_assert(array->is_dynamic, "Array is not dynamic");
    Array_Header *header = (Array_Header *)((char *)array + sizeof(void_Array));

    array->length -= 1;
    if (array->length % array->capacity == 0) {
        array->data = arena_realloc(
            header->arena, array->data,
            array->capacity * header->_item_size,
            (array->capacity - header->_capacity_grow) * header->_item_size
        );
        array->capacity += header->_capacity_grow;
    }
}

#endif
