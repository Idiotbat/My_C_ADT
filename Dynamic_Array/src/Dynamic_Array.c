#include "Dynamic_Array.h"

#include <stdlib.h>
#include <string.h>

static bool s_is_null_DArray(const DArray* const p_DArray);

static bool s_is_empty_DArray(const DArray* const p_DArray);

static void* s_resize_memory(void* const origin_data, size_t new_data_size,
	uintmax_t new_data_number);

static void s_insert_data(void* const target_m, size_t insert_target_index, 
	uintmax_t origin_number, const void* const source_m,
	size_t src_insert_index, uintmax_t insert_data_number, 
	size_t data_size);

static void s_remove_data(void* const data, uintmax_t origin_number,
	size_t remove_index, uintmax_t remove_number, size_t data_size);

static void s_change_data(void* const target_data, const void* const source_data,
	size_t change_size);

static void s_exchange_mem(void* const m_1, void* const m_2, void* const temp,
	size_t m_size);



void initialize_DArray(DArray* const p_DArray, size_t element_size)
{
	if (p_DArray == NULL) return;

	p_DArray->data = NULL;
	p_DArray->element_size = element_size;
	p_DArray->element_number = 0;
}

int copy_from_std_str(DArray* const p_DArray, const void* const p_std_arr, 
	size_t copy_element_size, uintmax_t copy_element_count)
{
	if (p_DArray == NULL || p_std_arr == NULL || copy_element_size == 0 ||
		copy_element_count == 0 || !s_is_empty_DArray(p_DArray)) return -1;

	initialize_DArray(p_DArray, copy_element_size);

	void* p_new_data = s_resize_memory(NULL, copy_element_size,
		copy_element_count);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, 0, p_std_arr, 0, copy_element_count, 
		copy_element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number = copy_element_count;

	return 0;
}

int copy_from_DArray(DArray* const p_target_DArray, 
	const DArray* const p_source_DArray)
{
	if (p_target_DArray == NULL || p_source_DArray == NULL ||
		s_is_empty_DArray(p_source_DArray) || !s_is_empty_DArray(p_target_DArray))
	{
		return -1;
	}

	initialize_DArray(p_target_DArray, p_source_DArray->element_size);

	void* p_new_data = s_resize_memory(NULL, p_source_DArray->element_size,
		p_source_DArray->element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, 0, p_source_DArray->data, 0,
		p_source_DArray->element_number, p_source_DArray->element_size);

	p_target_DArray->data = (char*)p_new_data;
	p_target_DArray->element_number = p_source_DArray->element_number;

	return 0;
}

int copy_part_from_std_arr(DArray* const p_DArray, const void* const p_std_arr, 
	size_t copy_element_size, size_t copy_start_index, uintmax_t copy_number)
{
	if (p_DArray == NULL || p_std_arr == NULL || copy_element_size == 0 ||
		copy_number == 0 || !s_is_empty_DArray(p_DArray)) return -1;

	initialize_DArray(p_DArray, copy_element_size);

	void* p_new_data = s_resize_memory(NULL, copy_element_size, copy_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, 0, p_std_arr, copy_start_index, copy_number,
		copy_element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number = copy_number;


	return 0;
}

int copy_part_from_DArray(DArray* const p_target_DArray, 
	const DArray* const p_source_DArray, size_t copy_start_index, 
	uintmax_t copy_number)
{
	if (p_target_DArray == NULL || p_source_DArray == NULL || copy_number == 0 ||
		!s_is_empty_DArray(p_target_DArray) || s_is_empty_DArray(p_source_DArray) ||
		copy_start_index + copy_number > p_source_DArray->element_number)
	{
		return -1;
	}


	initialize_DArray(p_target_DArray, p_source_DArray->element_size);

	void* p_new_data = s_resize_memory(NULL, p_source_DArray->element_size, 
		copy_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, 0, p_source_DArray->data, copy_start_index,
		copy_number, p_source_DArray->element_size);

	p_target_DArray->data = (char*)p_new_data;
	p_target_DArray->element_number = copy_number;

	return 0;
}

void clear_DArray(DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return;

	s_resize_memory(p_DArray->data, 0, 0);

	p_DArray->data = NULL;
	p_DArray->element_number = 0;
}

int push_back_to_DArray(DArray* const p_DArray, const void* const p_element)
{
	if (p_DArray == NULL || p_element == NULL || s_is_null_DArray(p_DArray)) 
		return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + 1);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, p_DArray->element_number,
		p_DArray->element_number, p_element, 0, 1, p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number++;

	return 0;
}

int push_front_to_DArray(DArray* const p_DArray, const void* const p_element)
{
	if (p_DArray == NULL || p_element == NULL || s_is_null_DArray(p_DArray))
		return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + 1);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, p_DArray->element_number, p_element, 0, 1, 
		p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number++;

	return 0;
}

int insert_to_DArray(DArray* const p_DArray, const void* const p_element, 
	size_t insert_index)
{
	if (p_DArray == NULL || p_element == NULL || s_is_null_DArray(p_DArray) ||
		insert_index > p_DArray->element_number) return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + 1);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, insert_index, p_DArray->element_number, p_element, 
		0, 1, p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number++;

	return 0;
}

void pop_back_from_DArray(DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return;

	s_remove_data(p_DArray->data, p_DArray->element_number,
		p_DArray->element_number - 1, 1, p_DArray->element_size);

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number - 1);

	if (p_new_data != NULL) {
		p_DArray->data = (char*)p_new_data;
	}
	p_DArray->element_number--;
}

void pop_front_from_DArray(DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return;

	s_remove_data(p_DArray->data, p_DArray->element_number,
		0, 1, p_DArray->element_size);

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number - 1);

	if (p_new_data != NULL) {
		p_DArray->data = (char*)p_new_data;
	}
	p_DArray->element_number--;
}

void remove_from_DArray(DArray* const p_DArray, size_t remove_index)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray) ||
		remove_index >= p_DArray->element_number) return;

	s_remove_data(p_DArray->data, p_DArray->element_number,
		remove_index, 1, p_DArray->element_size);

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number - 1);

	if (p_new_data != NULL) {
		p_DArray->data = (char*)p_new_data;
	}
	p_DArray->element_number--;
}

int push_back_DArray(DArray* const p_target_DArray, 
	const DArray* const p_source_DArray)
{
	if (p_target_DArray == NULL || p_source_DArray == NULL ||
		s_is_null_DArray(p_target_DArray) || s_is_empty_DArray(p_source_DArray) ||
		p_target_DArray->element_size != p_source_DArray->element_size) return -1;

	void* p_new_data = s_resize_memory(p_target_DArray->data, 
		p_target_DArray->element_size,
		p_target_DArray->element_number + p_source_DArray->element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, p_target_DArray->element_number,
		p_target_DArray->element_number, p_source_DArray->data, 
		0, p_source_DArray->element_number, p_target_DArray->element_size);

	p_target_DArray->data = (char*)p_new_data;
	p_target_DArray->element_number += p_source_DArray->element_number;

	return 0;
}

int push_back_std_arr(DArray* const p_DArray, const void* const p_std_arr, 
	uintmax_t add_element_number)
{
	if (p_DArray == NULL || p_std_arr == NULL || s_is_null_DArray(p_DArray) ||
		add_element_number == 0) return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + add_element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, p_DArray->element_number,
		p_DArray->element_number, p_std_arr,
		0, add_element_number, p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number += add_element_number;

	return 0;
}

int push_front_DArray(DArray* const p_target_DArray, 
	const DArray* const p_source_DArray)
{
	if (p_target_DArray == NULL || p_source_DArray == NULL ||
		s_is_null_DArray(p_target_DArray) || s_is_empty_DArray(p_source_DArray) ||
		p_target_DArray->element_size != p_source_DArray->element_size) return -1;

	void* p_new_data = s_resize_memory(p_target_DArray->data,
		p_target_DArray->element_size,
		p_target_DArray->element_number + p_source_DArray->element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, p_target_DArray->element_number,
		p_source_DArray->data, 0, p_source_DArray->element_number, 
		p_target_DArray->element_size);

	p_target_DArray->data = (char*)p_new_data;
	p_target_DArray->element_number += p_source_DArray->element_number;

	return 0;
}

int push_front_std_arr(DArray* const p_DArray, const void* const p_std_arr, 
	uintmax_t add_element_number)
{
	if (p_DArray == NULL || p_std_arr == NULL || s_is_null_DArray(p_DArray) ||
		add_element_number == 0) return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + add_element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, 0, p_DArray->element_number, p_std_arr,
		0, add_element_number, p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number += add_element_number;

	return 0;
}

int insert_DArray(DArray* const p_target_DArray, 
	const DArray* const p_source_DArray, size_t insert_index)
{
	if (p_target_DArray == NULL || p_source_DArray == NULL ||
		s_is_null_DArray(p_target_DArray) || s_is_empty_DArray(p_source_DArray) ||
		p_target_DArray->element_size != p_source_DArray->element_size ||
		insert_index > p_target_DArray->element_number) return -1;

	void* p_new_data = s_resize_memory(p_target_DArray->data,
		p_target_DArray->element_size,
		p_target_DArray->element_number + p_source_DArray->element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, insert_index, p_target_DArray->element_number,
		p_source_DArray->data, 0, p_source_DArray->element_number,
		p_target_DArray->element_size);

	p_target_DArray->data = (char*)p_new_data;
	p_target_DArray->element_number += p_source_DArray->element_number;

	return 0;
}

int insert_std_arr(DArray* const p_DArray, const void* const p_std_arr, 
	size_t insert_index, uintmax_t insert_element_number)
{
	if (p_DArray == NULL || p_std_arr == NULL || s_is_null_DArray(p_DArray) ||
		insert_element_number == 0) return -1;

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number + insert_element_number);

	if (p_new_data == NULL) return -3;

	s_insert_data(p_new_data, insert_index, p_DArray->element_number, p_std_arr,
		0, insert_element_number, p_DArray->element_size);

	p_DArray->data = (char*)p_new_data;
	p_DArray->element_number += insert_element_number;

	return 0;
}

void remove_part_from_DArray(DArray* const p_DArray, size_t remove_start_index, 
	uintmax_t remove_number)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray) || remove_number == 0 || 
		remove_start_index + remove_number > p_DArray->element_number) return;

	s_remove_data(p_DArray->data, p_DArray->element_number,
		remove_start_index, remove_number, p_DArray->element_size);

	void* p_new_data = s_resize_memory(p_DArray->data, p_DArray->element_size,
		p_DArray->element_number - remove_number);

	if (p_new_data != NULL) {
		p_DArray->data = (char*)p_new_data;
	}
	p_DArray->element_number -= remove_number;
}

void* get_first_of_DArray(const DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return NULL;

	return p_DArray->data;
}

void* get_last_of_DArray(const DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return NULL;

	return p_DArray->data + 
		(p_DArray->element_number - 1) * p_DArray->element_size;
}

void* get_index_of_DArray(const DArray* const p_DArray, size_t get_index)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray) ||
		get_index >= p_DArray->element_number) return NULL;

	return p_DArray->data + get_index * p_DArray->element_size;
}

void modify_index_of_DArray(DArray* const p_DArray, 
	const void* const p_new_value, size_t modify_index)
{
	if (p_DArray == NULL || p_new_value == NULL || s_is_empty_DArray(p_DArray) ||
		modify_index >= p_DArray->element_number) return;

	s_change_data(p_DArray->data + modify_index * p_DArray->element_size,
		p_new_value, p_DArray->element_size);
}

bool is_in_DArray(const DArray* const p_DArray, const void* const p_element, 
	int(*comparator)(const void*, const void*))
{
	if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
		s_is_empty_DArray(p_DArray)) return false;

	size_t temp_index = 0;
	for (; temp_index < p_DArray->element_number; temp_index++) {
		if ((comparator(p_DArray->data + temp_index * p_DArray->element_size,
			p_element)) == 0) return true;
	}
	return false;
}

uintmax_t number_in_DArray(const DArray* const p_DArray, 
	const void* const p_element, int(*comparator)(const void*, const void*))
{
	if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
		s_is_empty_DArray(p_DArray)) return 0;

	size_t temp_index = 0;
	uintmax_t temp_num = 0;
	for (; temp_index < p_DArray->element_number; temp_index++) {
		if ((comparator(p_DArray->data + temp_index * p_DArray->element_size,
			p_element)) == 0) temp_num++;
	}
	return temp_num;
}

size_t first_index_in_DArray(const DArray* const p_DArray, 
	const void* const p_element, int(*comparator)(const void*, const void*))
{
	if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
		s_is_empty_DArray(p_DArray)) return 0;

	size_t temp_index = 0;
	for (; temp_index < p_DArray->element_number; temp_index++) {
		if ((comparator(p_DArray->data + temp_index * p_DArray->element_size,
			p_element)) == 0) return temp_index;
	}
	return 0;
}

size_t last_index_in_DArray(const DArray* const p_DArray, 
	const void* const p_element, int(*comparator)(const void*, const void*))
{
	if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
		s_is_empty_DArray(p_DArray)) return 0;

	size_t temp_index = p_DArray->element_number - 1;
	for (; temp_index >= 0; temp_index--) {
		if ((comparator(p_DArray->data + temp_index * p_DArray->element_size,
			p_element)) == 0) return temp_index;
	}
	return 0;
}

void sort_DArray(DArray* const p_DArray, bool is_in_order, 
	int(*comparator)(const void*, const void*))
{
	if (p_DArray == NULL || comparator == NULL || s_is_empty_DArray(p_DArray) ||
		p_DArray->element_number < 2) return;

	void* temp = malloc(p_DArray->element_size);
	if (temp == NULL) return;

	int ret = 0;

	for (size_t i = 0; i < p_DArray->element_number - 1; i++) {
		for (size_t j = 0; j < p_DArray->element_number - 1 - i; j++) {
			ret = comparator(p_DArray->data + j * p_DArray->element_size,
				p_DArray->data + (j + 1) * p_DArray->element_size);
			if ((ret > 0 && is_in_order) || (ret < 0 && !is_in_order)) {
				s_exchange_mem(p_DArray->data + j * p_DArray->element_size,
					p_DArray->data + (j + 1) * p_DArray->element_size, temp,
					p_DArray->element_size);
			}
		}
	}

	free(temp);
}

void reverse_DArray(DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray) ||
		p_DArray->element_number < 2) return;

	void* temp = malloc(p_DArray->element_size);
	if (temp == NULL) return;

	size_t temp_num = p_DArray->element_number / 2;
	for (size_t i = 0; i < temp_num; i++) {
		s_exchange_mem(p_DArray->data + i * p_DArray->element_size,
			p_DArray->data + (p_DArray->element_number - 1 - i) *
			p_DArray->element_size, temp, p_DArray->element_size);
	}
}

uintmax_t element_number_of_DArray(const DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return 0;
	return p_DArray->element_number;
}

bool is_DArray_empty(const DArray* const p_DArray)
{
	if (p_DArray == NULL || s_is_empty_DArray(p_DArray)) return true;
	return false;
}

void traverse_DArray(const DArray* const p_DArray, void(*traversal)(void*))
{
	if (p_DArray == NULL || traversal == NULL || s_is_empty_DArray(p_DArray)) return;

	for (size_t i = 0; i < p_DArray->element_number; i++) {
		traversal(p_DArray->data + i * p_DArray->element_size);
	}
}



bool s_is_null_DArray(const DArray* const p_DArray)
{
	if (p_DArray->element_size == 0) return true;
	else return false;
}

bool s_is_empty_DArray(const DArray* const p_DArray)
{
	if (p_DArray->element_size == 0 || p_DArray->data == NULL ||
		p_DArray->element_number == 0) return true;
	else return false;
}

void* s_resize_memory(void* const origin_data, size_t new_data_size, 
	uintmax_t new_data_number)
{
	return realloc(origin_data, new_data_size * new_data_number);
}

void s_insert_data(void* const target_m, size_t insert_target_index, 
	uintmax_t origin_number, const void* const source_m, size_t src_insert_index, 
	uintmax_t insert_data_number, size_t data_size)
{

	memmove((char*)target_m + 
		(insert_target_index + insert_data_number) * data_size,
		(char*)target_m + insert_target_index * data_size, 
		(origin_number - insert_target_index) * data_size);

	memmove((char*)target_m + insert_target_index * data_size,
		(char*)source_m + src_insert_index * data_size,
		insert_data_number * data_size);
}

void s_remove_data(void* const data, uintmax_t origin_number, 
	size_t remove_index, uintmax_t remove_number, size_t data_size)
{
	memmove((char*)data + remove_index * data_size,
		(char*)data + (remove_index + remove_number) * data_size,
		(origin_number - remove_index - remove_number) * data_size);
}

void s_change_data(void* const target_data, const void* const source_data, 
	size_t change_size)
{
	memmove(target_data, source_data, change_size);
}

void s_exchange_mem(void* const m_1, void* const m_2, void* const temp, 
	size_t m_size)
{
	memmove(temp, m_1, m_size);
	memmove(m_1, m_2, m_size);
	memmove(m_2, temp, m_size);
}

