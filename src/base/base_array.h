#ifndef CTTP_BASE_ARRAY_H
#define CTTP_BASE_ARRAY_H

typedef struct Array_ {
    void *data;
    size_t length;
    size_t capacity;
    size_t _item_size;
    size_t _capacity_grow;
    Arena *arena;
} Array_;

#ifndef ARRAY_INIT_CAPACITY
    #define ARRAY_INIT_CAPACITY 32
#endif

#define array_init(Type, arena) (Type##_Array *) array_init_(sizeof(Type), ARRAY_INIT_CAPACITY, arena)

#define Array(Type) \
typedef struct Type##_Array { \
    Type *data; \
    size_t length; \
    size_t capacity; \
    size_t _item_size; \
    size_t _capacity_grow; \
    Arena *arena; \
} Type##_Array \


// (Type##Array *) array_init_(sizeof(Type), ARRAY_INIT_CAPACITY, arena)

internal
Array_ *array_init_(size_t item_size, size_t capacity, Arena *arena) {
    size_t size = item_size * capacity;
    Array_ *array = arena_alloc(arena,  + sizeof(Array_));
    if (array == 0) {
        assert(0 && "Array Header allocation failed");
    }

    array->data = arena_alloc(arena, capacity * item_size);
    if (array->data == 0) {
        assert(0 && "Array allocation failed");
    }
    array->arena = arena;
    array->length = 0;
    array->capacity = capacity;
    array->_capacity_grow = capacity;
    array->_item_size = item_size;

    return array;
}

#define array_push(array, obj) array_push_(array, (void *)(obj))

//TODO: Replace with a more efficient method
#define array_push_many(array, new_items, new_items_count) \
for (u64 i = 0; i < new_items_count; i++) {                \
    array_push(array, new_items[i]);                    \
}

internal
void array_push_(Array_ *array, void *item) {
    assert(item != NULL && "Tried to append NULL");
    assert(sizeof(*item) != array->_item_size && "Item has a different size");
    array->length += 1;
    if (array->length >= array->capacity) {
        array->data = arena_realloc(
            array->arena, array->data,
            array->capacity * array->_item_size,
            (array->capacity + array->_capacity_grow) * array->_item_size
        );
        array->capacity += array->_capacity_grow;
    }
    arena_memcpy((array->data + (array->length * array->_item_size)), item, array->_item_size);
}

internal
void array_pop(Array_ *array) {
    assert(array->length == 0 && "No Item Left");
    array->length -= 1;
    if (array->length % array->capacity == 0) {
        array->data = arena_realloc(
            array->arena, array->data,
            array->capacity * array->_item_size,
            (array->capacity - array->_capacity_grow) * array->_item_size
        );
        array->capacity += array->_capacity_grow;
    }
}

#endif
