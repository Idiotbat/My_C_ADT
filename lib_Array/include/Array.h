#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// ADT-Array struct defination
typedef struct Array Array;


// API
Array* create_Array(
    size_t type_size,
    uintmax_t element_number
);

void initialize_Array(
    const Array* p_Array
);

void destroy_Array(
    Array* p_Array
);

bool resize_Array(
    Array* p_Array,
    uintmax_t element_number
);

size_t type_size_of_Array(
    const Array* p_Array
);

uintmax_t element_number_of_Array(
    const Array* p_Array
);

size_t size_of_Array(
    const Array* p_Array
);

bool is_empty_Array(
    const Array* p_Array
);

void set_element_of_Array(
    const Array* p_Array,
    size_t index,
    void* new_value
);

void* get_element_of_Array(
    const Array* p_Array,
    size_t index
);

void reverse_Array(
    const Array* p_Array
);

void traverse_Array(
    const Array* p_Array,
    void (*traversal)(void*)
);

#endif