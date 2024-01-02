#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 动态数组——ADT类型定义
typedef struct Dynamic_Array {
	char* data;
	size_t element_size;
	uintmax_t element_number;
}DArray;


// 初始化一个DArray
void initialize_DArray(
	DArray* const p_DArray,
	size_t element_size
);

// 复制一个C标准数组到一个空DArray中
int copy_from_std_str(
	DArray* const p_DArray,
	const void* const p_std_arr,
	size_t copy_element_size,
	uintmax_t copy_element_count
);

// 复制一个DArray到一个空DArray中
int copy_from_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray
);

// 复制一个C标准数组中的一部分到一个空DArray中
int copy_part_from_std_arr(
	DArray* const p_DArray,
	const void* const p_std_arr,
	size_t copy_element_size,
	size_t copy_start_index,
	uintmax_t copy_number
);

// 复制一个DArray中的一部分到一个空DArray中
int copy_part_from_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray,
	size_t copy_start_index,
	uintmax_t copy_number
);

// 清空一个DArray
void clear_DArray(
	DArray* const p_DArray
);

// 在一个DArray的末尾添加一个元素
int push_back_to_DArray(
	DArray* const p_DArray,
	const void* const p_element
);

// 在一个DArray的开头添加一个元素
int push_front_to_DArray(
	DArray* const p_DArray,
	const void* const p_element
);

// 在一个DArray的指定位置插入一个元素
int insert_to_DArray(
	DArray* const p_DArray,
	const void* const p_element,
	size_t insert_index
);

// 从一个DArray的末尾删除一个元素
void pop_back_from_DArray(
	DArray* const p_DArray
);

// 从一个DArray的开头删除一个元素
void pop_front_from_DArray(
	DArray* const p_DArray
);

// 从一个DArray的指定位置删除一个元素
void remove_from_DArray(
	DArray* const p_DArray,
	size_t remove_index
);

// 将一个DArray追加到另一个DArray后面
int push_back_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray
);

// 将一个C数组追加到另一个DArray后面
int push_back_std_arr(
	DArray* const p_DArray,
	const void* const p_std_arr,
	uintmax_t add_element_number
);

// 将一个DArray追加到另一个DArray前面
int push_front_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray
);

// 将一个C数组追加到另一个DArray前面
int push_front_std_arr(
	DArray* const p_DArray,
	const void* const p_std_arr,
	uintmax_t add_element_number
);

// 将一个DArray插入另一个DArray中的指定索引处
int insert_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray,
	size_t insert_index
);

// 将一个C数组插入另一个DArray中的指定索引处
int insert_std_arr(
	DArray* const p_DArray,
	const void* const p_std_arr,
	size_t insert_index,
	uintmax_t insert_element_number
);

// 删除一个DArray的一部分
void remove_part_from_DArray(
	DArray* const p_DArray,
	size_t remove_start_index,
	uintmax_t remove_number
);

// 获取一个DArray中的第一个元素的指针
void* get_first_of_DArray(
	const DArray* const p_DArray
);

// 获取一个DArray中的最后一个元素的指针
void* get_last_of_DArray(
	const DArray* const p_DArray
);

// 获取一个DArray中指定位置的元素的指针
void* get_index_of_DArray(
	const DArray* const p_DArray,
	size_t get_index
);

// 修改一个DArray中指定位置的元素的值
void modify_index_of_DArray(
	DArray* const p_DArray,
	const void* const p_new_value,
	size_t modify_index
);

// 检查一个DArray中是否包含指定元素
bool is_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个DArray中出现的次数
uintmax_t number_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个DArray中第一次出现的索引
size_t first_index_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个DArray中最后一次出现的索引
size_t last_index_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void*, const void*)
);

// 对一个DArray排序
void sort_DArray(
	DArray* const p_DArray,
	bool is_in_order,
	int(*comparator)(const void*, const void*)
);

// 将一个DArray顺序颠倒
void reverse_DArray(
	DArray* const p_DArray
);

// 返回一个DArray中的元素个数
uintmax_t element_number_of_DArray(
	const DArray* const p_DArray
);

// 判断一个DArray是否为空
bool is_DArray_empty(
	const DArray* const p_DArray
);

// 遍历一个DArray
void traverse_DArray(
	const DArray* const p_DArray,
	void (*traversal)(void*)
);