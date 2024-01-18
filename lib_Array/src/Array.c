#include "Array.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Array
{
    void* data;
    size_t type_size;
    uintmax_t element_number;
};

// API
Array* create_Array(size_t type_size, uintmax_t element_number) {
    if (type_size == 0 || element_number == 0) return NULL;

    Array* pNewArray = (Array*)malloc(sizeof(Array));
    if (pNewArray == NULL) return NULL;

    void* pNewData = calloc(element_number, type_size);
    if (pNewData == NULL) {
        free(pNewArray);
        return NULL;
    }
    else {
        *pNewArray = (Array){ pNewData, type_size, element_number };
        return pNewArray;
    }
}


void initialize_Array(const Array* p_Array) {
    if (p_Array == NULL || p_Array->data == NULL) return;

    memset(p_Array->data, 0, p_Array->element_number * p_Array->type_size);
}

void destroy_Array(Array* p_Array) {
    if (p_Array == NULL) return;

    if (p_Array->data != NULL) free(p_Array->data);

    free(p_Array);
}

bool resize_Array(Array* p_Array, uintmax_t element_number) {
    if (p_Array == NULL) return false;

    if (element_number == 0) return false;

    void* pNewData = calloc(element_number, p_Array->type_size);
    if (pNewData == NULL) return false;
    else {
        size_t temp = (((p_Array->element_number) < (element_number)) ?
            (p_Array->element_number) : (element_number));
        memcpy(pNewData, p_Array->data, 
            temp * p_Array->type_size);
        free(p_Array->data);
        p_Array->data = pNewData;
        p_Array->element_number = element_number;
        return true;
    }
}


size_t type_size_of_Array(const Array* p_Array) {
    if (p_Array == NULL) return 0;
    return p_Array->type_size;
}

uintmax_t element_number_of_Array(const Array* p_Array) {
    if (p_Array == NULL) return 0;
    return p_Array->element_number;
}


size_t size_of_Array(const Array* p_Array) {
    if (p_Array == NULL) return 0;
    return p_Array->element_number * p_Array->type_size;
}


bool is_empty_Array(const Array* p_Array) {
    if (p_Array == NULL) return true;

    return p_Array->data == NULL ? true : false;
}


void set_element_of_Array(const Array* p_Array, size_t index, void* new_value) {
    if (p_Array == NULL || new_value == NULL) return;

    if (index >= p_Array->element_number) return;

    memcpy((char*)p_Array->data + index * p_Array->type_size, 
        new_value, p_Array->type_size);
}

void* get_element_of_Array(const Array* p_Array, size_t index) {
    if (p_Array == NULL) return NULL;
    
    if (index >= p_Array->element_number) return NULL;

    return (char*)p_Array->data + index * p_Array->type_size;
}


void reverse_Array(const Array* p_Array) {
    if (p_Array == NULL) return;

    void* temp = malloc(p_Array->type_size);
    if (temp == NULL) return;

    for (int i = 0; i < p_Array->element_number / 2; i++) {
        memcpy(temp, (char*)p_Array->data + i * p_Array->type_size,
            p_Array->type_size);
        memcpy((char*)p_Array->data + i * p_Array->type_size, 
            (char*)p_Array->data + (p_Array->element_number - 1 - i) *
            p_Array->type_size, p_Array->type_size);
        memcpy((char*)p_Array->data + (p_Array->element_number - 1 - i) *
            p_Array->type_size, temp, p_Array->type_size);
    }
}


void traverse_Array(const Array* p_Array, void (*traversal)(void*)) {
    if (p_Array == NULL || traversal == NULL) return;

    for (int i = 0; i < p_Array->element_number; i++)
    {
        traversal((char*)p_Array->data + i * p_Array->type_size);
    }
}







