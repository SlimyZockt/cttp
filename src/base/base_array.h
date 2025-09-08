#ifndef CTTP_BASE_ARRAY_H
#define CTTP_BASE_ARRAY_H

typedef struct Array {
    void *data;
    size_t length;
    size_t capacity;
    size_t _item_size;
    size_t _capacity_grow;
    Arena *arena;
} Array;

#ifndef ARRAY_INIT_CAPACITY
    #define ARRAY_INIT_CAPACITY 32
#endif

#define array(Type, arena) array_init(sizeof(Type), ARRAY_INIT_CAPACITY, arena)

internal
Array array_init(size_t item_size, size_t capacity, Arena *arena) {
    void *ptr = 0;
    size_t size = item_size * capacity;

    return ( Array ){
        .capacity = capacity,
        ._capacity_grow = capacity,
        .length = 0,
        .arena = arena,
        .data = arena_alloc(arena, capacity * item_size),
        ._item_size = item_size,
    };
}

#define array_append(array, obj) array_append_(array, (void *)(&obj))

internal
void array_append_(Array *array, void *item) {
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

#endif
