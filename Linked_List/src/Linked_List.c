#include "Linked_List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bool s_is_null_LList(const LList* const p_LList);

static bool s_is_empty_LList(const LList* const p_LList);

static void s_clear_LNode(LNode* const p_LNode);

static void s_LNode_number_increase(LList* const p_LList);

static void s_LNode_number_reduce(LList* const p_LList);

static LNode* s_make_LNode(size_t element_size, const void* const new_data);

static void s_add_LNode(LList* const p_LList, LNode* const p_LNode,
	size_t add_index);

static LNode* s_LNode_of_index(const LList* const p_LList, size_t node_index);

static void s_remove_LNode(LList* const p_LList, size_t remove_index,
	uintmax_t remove_number);

static int s_add_LList(LList* const p_target_LList, size_t add_index,
	const LList* const p_source_LList, size_t src_start_index, uintmax_t add_number);

static void s_traverse(const LNode* const p_LNode, void(*traversal)(void*));

static void s_reverse(LList* const p_LList);


void initialize_LList(LList* const p_LList, size_t element_size) {
	if (p_LList == NULL) return;

	p_LList->head = NULL;
	p_LList->tail = NULL;
	p_LList->element_size = element_size;
	p_LList->node_number = 0;
}

void clear_LList(LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return;

	LNode* p_node = NULL;

	while (p_LList->head != NULL) {
		p_node = p_LList->head;
		p_LList->head = p_LList->head->next;
		s_clear_LNode(p_node);
		s_LNode_number_reduce(p_LList);
	}

	p_LList->tail = NULL;
}

int push_back_to_LList(LList* const p_LList, const void* const new_data) {
	if (p_LList == NULL || new_data == NULL || s_is_null_LList(p_LList)) return -1;

	LNode* p_new_node = s_make_LNode(p_LList->element_size, new_data);

	if (p_new_node == NULL) return -3;

	s_add_LNode(p_LList, p_new_node, p_LList->node_number);

	return 0;
}

int push_front_to_LList(LList* const p_LList, const void* const new_data) {
	if (p_LList == NULL || new_data == NULL || s_is_null_LList(p_LList)) return -1;

	LNode* p_new_node = s_make_LNode(p_LList->element_size, new_data);

	if (p_new_node == NULL) return -3;

	s_add_LNode(p_LList, p_new_node, 0);

	return 0;
}

int insert_to_LList(LList* const p_LList, const void* const new_data,
	size_t insert_index)
{
	if (p_LList == NULL || new_data == NULL || s_is_null_LList(p_LList)) return -1;

	if (insert_index > p_LList->node_number) return -1;


	LNode* p_new_node = s_make_LNode(p_LList->element_size, new_data);

	if (p_new_node == NULL) return -3;

	s_add_LNode(p_LList, p_new_node, insert_index);

	return 0;
}

void pop_back_from_LList(LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return;

	s_remove_LNode(p_LList, p_LList->node_number - 1, 1);
}

void pop_front_from_LList(LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return;

	s_remove_LNode(p_LList, 0, 1);
}

void remove_from_LList(LList* const p_LList, size_t remove_index) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return;

	s_remove_LNode(p_LList, remove_index, 1);
}

int push_back_LList(LList* const p_target_LList, const LList* const p_source_LList) {
	if (p_target_LList == NULL || p_source_LList == NULL ||
		s_is_null_LList(p_target_LList) || s_is_empty_LList(p_target_LList))
		return -1;

	if (p_target_LList->element_size != p_source_LList->element_size)
		return -1;

	return s_add_LList(p_target_LList, p_target_LList->node_number, p_source_LList,
		0, p_source_LList->node_number);
}

int push_front_LList(LList* const p_target_LList, const LList* const p_source_LList) {
	if (p_target_LList == NULL || p_source_LList == NULL ||
		s_is_null_LList(p_target_LList) || s_is_empty_LList(p_target_LList))
		return -1;

	if (p_target_LList->element_size != p_source_LList->element_size)
		return -1;

	return s_add_LList(p_target_LList, 0, p_source_LList, 0,
		p_source_LList->node_number);
}

int insert_LList(LList* const p_target_LList, const LList* const p_source_LList,
	size_t insert_index)
{
	if (p_target_LList == NULL || p_source_LList == NULL ||
		s_is_null_LList(p_target_LList) || s_is_empty_LList(p_target_LList))
		return -1;

	if (p_target_LList->element_size != p_source_LList->element_size)
		return -1;

	if (insert_index > p_target_LList->node_number) return -1;

	return s_add_LList(p_target_LList, insert_index, p_source_LList, 0,
		p_source_LList->node_number);
}

void remove_part_from_LList(LList* const p_LList, size_t remove_start_index,
	uintmax_t remove_number)
{
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return;

	if (remove_start_index + remove_number - 1 >= p_LList->node_number) return;

	s_remove_LNode(p_LList, remove_start_index, remove_number);
}

int copy_LList(LList* const p_target_LList, const LList* const p_source_LList) {
	if (p_target_LList == NULL || p_source_LList == NULL ||
		!s_is_empty_LList(p_target_LList) || s_is_empty_LList(p_source_LList)) return -1;

	initialize_LList(p_target_LList, p_source_LList->element_size);
	return s_add_LList(p_target_LList, 0, p_source_LList, 0,
		p_source_LList->node_number);
}

int copy_part_LList(LList* const p_target_LList, const LList* const p_source_LList,
	size_t copy_strat_index, uintmax_t copy_number)
{
	if (p_target_LList == NULL || p_source_LList == NULL ||
		!s_is_empty_LList(p_target_LList) || s_is_empty_LList(p_source_LList)) return -1;

	initialize_LList(p_target_LList, p_source_LList->element_size);

	return s_add_LList(p_target_LList, 0, p_source_LList, copy_strat_index,
		copy_number);
}

void traverse_LList(const LList* const p_LList, void(*traversal)(void*)) {
	if (p_LList == NULL || traversal == NULL || s_is_empty_LList(p_LList)) return;

	s_traverse(p_LList->head, traversal);
}

void reverse_LList(LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList) || p_LList->node_number < 2)
		return;

	s_reverse(p_LList);
}

bool is_LList_empty(const LList* const p_LList) {
	if (p_LList == NULL) return true;

	return s_is_empty_LList(p_LList);
}

uintmax_t element_number_of_LList(const LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return 0;

	return p_LList->node_number;
}

void* get_first_of_LList(const LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return NULL;

	return p_LList->head->data;
}

void* get_last_of_LList(const LList* const p_LList) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return NULL;

	return p_LList->tail->data;
}

void* get_index_of_LList(const LList* const p_LList, size_t get_index) {
	if (p_LList == NULL || s_is_empty_LList(p_LList)) return NULL;

	if (get_index >= p_LList->node_number) return NULL;

	return s_LNode_of_index(p_LList, get_index)->data;
}

void modify_index_of_LList(const LList* const p_LList, size_t modify_index,
	const void* const p_new_data)
{
	if (p_LList == NULL || p_new_data == NULL || s_is_empty_LList(p_LList)) return;

	if (modify_index >= p_LList->node_number) return;

	memmove(s_LNode_of_index(p_LList, modify_index)->data, p_new_data,
		p_LList->element_size);
}

bool is_in_LList(const LList* const p_LList, const void* const data,
	int(*comparator)(const void*, const void*))
{
	if (p_LList == NULL || data == NULL || comparator == NULL ||
		s_is_empty_LList(p_LList)) return false;

	LNode* p_node = p_LList->head;
	while (p_node != NULL) {
		if (comparator(p_node->data, data) == 0) return true;
		p_node = p_node->next;
	}
	return false;
}

uintmax_t number_in_LList(const LList* const p_LList, const void* const data,
	int(*comparator)(const void*, const void*))
{
	if (p_LList == NULL || data == NULL || comparator == NULL ||
		s_is_empty_LList(p_LList)) return 0;

	uintmax_t num = 0;
	LNode* p_node = p_LList->head;
	while (p_node != NULL) {
		if (comparator(p_node->data, data) == 0) num++;
		p_node = p_node->next;
	}

	return num;
}

size_t first_index_in_LList(const LList* const p_LList, const void* const data,
	int(*comparator)(const void*, const void*))
{
	if (p_LList == NULL || data == NULL || comparator == NULL ||
		s_is_empty_LList(p_LList)) return 0;

	size_t index = 0;
	LNode* p_node = p_LList->head;
	while (p_node != NULL) {
		if (comparator(p_node->data, data) == 0) return index;
		p_node = p_node->next;
		index++;
	}

	return 0;
}

size_t last_index_in_LList(const LList* const p_LList, const void* const data,
	int(*comparator)(const void*, const void*))
{
	if (p_LList == NULL || data == NULL || comparator == NULL ||
		s_is_empty_LList(p_LList)) return 0;

	size_t index = p_LList->node_number - 1;
	LNode* p_node = p_LList->tail;
	while (p_node != NULL) {
		if (comparator(p_node->data, data) == 0) return index;
		p_node = p_node->previous;
		index--;
	}

	return 0;
}

void sort_LList(LList* const p_LList, bool is_in_order,
	int(*comparator)(const void*, const void*))
{
	if (p_LList == NULL || comparator == NULL || s_is_empty_LList(p_LList)) return;

	if (p_LList->node_number < 2) return;
	LNode* p_node = p_LList->head;
	void* temp = NULL;
	int cmp_ret = 0;
	for (size_t i = 0; i < p_LList->node_number - 1; i++) {

		p_node = p_LList->head;

		for (size_t j = 0; j < p_LList->node_number - 1 - i; j++) {

			cmp_ret = comparator(p_node->data, p_node->next->data);

			if ((is_in_order && cmp_ret > 0) || (!is_in_order && cmp_ret < 0))
			{
				temp = p_node->data;
				p_node->data = p_node->next->data;
				p_node->next->data = temp;
			}

			p_node = p_node->next;
		}
	}
}

bool s_is_null_LList(const LList* const p_LList) {
	if (p_LList->element_size == 0) return true;
	else return false;
}

bool s_is_empty_LList(const LList* const p_LList) {
	if (p_LList->element_size == 0 || p_LList->node_number == 0 ||
		p_LList->head == NULL || p_LList->tail == NULL) return true;
	else return false;
}

void s_clear_LNode(LNode* const p_LNode) {
	if (p_LNode->data != NULL) free(p_LNode->data);

	free(p_LNode);
}

void s_LNode_number_increase(LList* const p_LList) {
	p_LList->node_number++;
}

void s_LNode_number_reduce(LList* const p_LList) {
	p_LList->node_number--;
}

LNode* s_make_LNode(size_t element_size, const void* const new_data) {
	void* p_data = calloc(1, element_size);
	if (p_data == NULL) return NULL;

	LNode* p_new_node = (LNode*)calloc(1, sizeof(LNode));
	if (p_new_node == NULL) {
		free(p_data);
		return NULL;
	}

	if (new_data != NULL) {
		memmove(p_data, new_data, element_size);
	}

	p_new_node->data = p_data;
	p_new_node->previous = NULL;
	p_new_node->next = NULL;

	return p_new_node;
}

void s_add_LNode(LList* const p_LList, LNode* const p_LNode, size_t add_index) {
	LNode* temp = s_LNode_of_index(p_LList, add_index);
	if (temp == NULL) {
		if (p_LList->tail == NULL) {
			p_LList->head = p_LNode;
		}
		else {
			p_LList->tail->next = p_LNode;
			p_LNode->previous = p_LList->tail;
		}
		p_LList->tail = p_LNode;
	}
	else {
		p_LNode->next = temp;
		p_LNode->previous = temp->previous;

		temp->previous = p_LNode;
		if (p_LNode->previous == NULL) {
			p_LList->head = p_LNode;
		}
		else {
			p_LNode->previous->next = p_LNode;
		}
	}
	s_LNode_number_increase(p_LList);
}

LNode* s_LNode_of_index(const LList* const p_LList, size_t node_index) {
	if (node_index >= p_LList->node_number) return NULL;

	if (node_index == 0) return p_LList->head;

	size_t mid_index = p_LList->node_number / 2;
	if (node_index <= mid_index) {
		LNode* p_node = p_LList->head;
		for (size_t i = 0; i < node_index; i++) {
			p_node = p_node->next;
		}
		return p_node;
	}
	else {
		LNode* p_node = p_LList->tail;
		size_t num = p_LList->node_number - 1 - node_index;
		for (size_t i = 0; i < num; i++) {
			p_node = p_node->previous;
		}
		return p_node;
	}
}

void s_remove_LNode(LList* const p_LList, size_t remove_index,
	uintmax_t remove_number)
{
	LNode* temp1 = s_LNode_of_index(p_LList, remove_index);
	LNode* temp2 = NULL;
	for (size_t i = 0; i < remove_number; i++) {
		if (temp1 != NULL) {
			temp2 = temp1->next;
			if (temp1->previous == NULL) {
				p_LList->head = temp1->next;
			}
			else {
				temp1->previous->next = temp1->next;
			}

			if (temp1->next == NULL) {
				p_LList->tail = temp1->previous;
			}
			else {
				temp1->next->previous = temp1->previous;
			}
			s_clear_LNode(temp1);
			s_LNode_number_reduce(p_LList);

			temp1 = temp2;
		}
	}
}

int s_add_LList(LList* const p_target_LList, size_t add_index,
	const LList* const p_source_LList, size_t src_start_index, uintmax_t add_number)
{
	LNode* temp = s_LNode_of_index(p_source_LList, src_start_index);

	for (size_t i = 0; i < add_number; i++) {
		int ret = insert_to_LList(p_target_LList, temp->data, add_index + i);
		if (ret != 0) return ret;
		temp = temp->next;
	}
	return 0;
}

void s_traverse(const LNode* const p_LNode, void(*traversal)(void*))
{
	if (p_LNode != NULL) {
		traversal(p_LNode->data);
		s_traverse(p_LNode->next, traversal);
	}
}

void s_reverse(LList* const p_LList) {
	uintmax_t num = p_LList->node_number / 2;
	LNode* p_node_1 = p_LList->head;
	LNode* p_node_2 = p_LList->tail;
	void* temp = NULL;
	for (size_t i = 0; i < num; i++) {
		temp = p_node_1->data;
		p_node_1->data = p_node_2->data;
		p_node_2->data = temp;

		p_node_1 = p_node_1->next;
		p_node_2 = p_node_2->previous;
	}
}