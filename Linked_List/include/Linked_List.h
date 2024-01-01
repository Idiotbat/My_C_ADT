#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// 节点
/*
data，指向该节点的数据
previous，指向上一个节点的指针
next，指向下一个节点的指针
*/
typedef struct List_Node {
	void* data;
	struct List_Node* previous;
	struct List_Node* next;
}LNode;

// 链表
/*
head，指向链表的头节点
tail，指向链表的尾节点
element_size，链表每个元素的大小（单位：字节）
node_number，链表的节点个数
*/
typedef struct Linked_List {
	LNode* head;
	LNode* tail;
	size_t element_size;
	uintmax_t node_number;
}LList;


// API

// 初始化一个LList
void initialize_LList(
	LList* const p_LList,
	size_t element_size
);

// 清空一个LList
void clear_LList(
	LList* const p_LList
);

// 在一个LList的末尾追加一个元素
int push_back_to_LList(
	LList* const p_LList,
	const void* const new_data
);

// 在一个LList的开头追加一个元素
int push_front_to_LList(
	LList* const p_LList,
	const void* const new_data
);

// 在一个LList的指定位置插入一个元素
int insert_to_LList(
	LList* const p_LList,
	const void* const new_data,
	size_t insert_index
);

// 从一个LList的末尾删除一个元素
void pop_back_from_LList(
	LList* const p_LList
);

// 从一个LList的开头删除一个元素
void pop_front_from_LList(
	LList* const p_LList
);

// 从一个LList的指定位置删除一个元素
void remove_from_LList(
	LList* const p_LList,
	size_t remove_index
);

// 在一个LList的末尾追加另一个LList
int push_back_LList(
	LList* const p_target_LList,
	const LList* const p_source_LList
);

// 在一个LList的开头追加另一个LList
int push_front_LList(
	LList* const p_target_LList,
	const LList* const p_source_LList
);

// 在一个LList的指定位置插入另一个LList
int insert_LList(
	LList* const p_target_LList,
	const LList* const p_source_LList,
	size_t insert_index
);

// 从一个LList中删除部分元素
void remove_part_from_LList(
	LList* const p_LList,
	size_t remove_start_index,
	uintmax_t remove_number
);

// 把一个LList复制到另一个空的且element_size为0的LList中
int copy_LList(
	LList* const p_target_LList,
	const LList* const p_source_LList
);

// 把一个LList的一部分复制到另一个空的且element_size为0的LList中
int copy_part_LList(
	LList* const p_target_LList,
	const LList* const p_source_LList,
	size_t copy_strat_index,
	uintmax_t copy_number
);

// 遍历一个LList
void traverse_LList(
	const LList* const p_LList,
	void(*traversal)(void*)
);

// 将一个LList反向
void reverse_LList(
	LList* const p_LList
);

// 判断一个LList是否为空
bool is_LList_empty(
	const LList* const p_LList
);

// 返回一个LList的元素个数
uintmax_t element_number_of_LList(
	const LList* const p_LList
);

// 返回一个LList的首元素指针
void* get_first_of_LList(
	const LList* const p_LList
);

// 返回一个LList的尾元素指针
void* get_last_of_LList(
	const LList* const p_LList
);

// 返回一个LList的指定位置元素指针
void* get_index_of_LList(
	const LList* const p_LList,
	size_t get_index
);

// 修改一个LList的指定位置元素
void modify_index_of_LList(
	const LList* const p_LList,
	size_t modify_index,
	const void* const new_data
);

// 判断一个元素是否在一个LList中
bool is_in_LList(
	const LList* const p_LList,
	const void* const data,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个LList中出现次数
uintmax_t number_in_LList(
	const LList* const p_LList,
	const void* const data,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个LList中第一次出现的索引
size_t first_index_in_LList(
	const LList* const p_LList,
	const void* const data,
	int(*comparator)(const void*, const void*)
);

// 返回一个元素在一个LList中最后一次出现的索引
size_t last_index_in_LList(
	const LList* const p_LList,
	const void* const data,
	int(*comparator)(const void*, const void*)
);

// 对一个LList排序
void sort_LList(
	LList* const p_LList,
	bool is_in_order,
	int(*comparator)(const void*, const void*)
);
