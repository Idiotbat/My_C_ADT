#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <inttypes.h>
#include <stdbool.h>


// 动态数组——ADT类型定义
/*
data：指向动态数组的指针
count：动态数组的元素个数
type_size：动态数组的类型大小（单位：字节）
*/
typedef struct Dynamic_Array {
	char* data;
	uintmax_t count;
	size_t type_size;
}DArray;


// 动态数组——API函数声明

// 1-创建、清理

// 创建一个空的，指定类型大小的DArray
/*
参数：
	type_size：所创建DArray的类型大小（单位：字节）
返回值：
	一个空的，类型大小为type_size的DArray
*/
DArray create_DArray(
	size_t type_size
);

// 复制一个C数组，来创建一个DArray
/*
参数：
	p_std_arr：被复制C数组的指针
	element_count：被复制C数组的元素个数
	element_size：被复制C数组的类型大小
返回值：
	一个内容与被复制C数组相同的DArray

*/
DArray copy_from_std_arr(
	const void* const p_std_arr,
	uintmax_t element_count,
	size_t element_size
);

// 复制一个DArray，来创建一个DArray
/*
参数：
	p_DArray：被复制DArray的指针
返回值：
	一个内容与被复制DArray相同的DArray
*/
DArray copy_from_DArray(
	const DArray* const p_DArray
);

// 复制一个C数组中的一部分，来创建一个DArray
/*
参数：
	p_std_arr：被复制C数组的指针
	element_size：被复制C数组的类型大小（单位：字节）
	start_index：被复制部分起始位置的索引
	end_index：被复制部分结束位置的索引
返回值：
	一个内容与被复制C数组的部分相同的DArray
*/
DArray copy_part_from_std_arr(
	const void* const p_std_arr,
	size_t element_size,
	uintmax_t start_index,
	uintmax_t end_index
);

// 复制一个DArray中的一部分，来创建一个DArray
/*
参数：
	p_DArray：被复制DArray的指针
	start_index：被复制部分起始位置的索引
	end_index：被复制部分结束位置的索引
返回值：
	一个内容与被复制的部分相同的DArray
*/
DArray copy_part_from_DArray(
	const DArray* const p_DArray,
	uintmax_t start_index,
	uintmax_t end_index
);

// 清空一个DArray
/*
参数：
	p_DArray：要清空的DArray的指针
*/
void clear_DArray(
	DArray* const p_DArray
);


// 2-添加、删除元素

// 在一个DArray的末尾添加一个元素
/*
参数：
	p_DArray：要添加元素的DArray的指针
	p_element：被添加元素的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示添加成功
		非0表示添加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int push_back_to_DArray(
	DArray* const p_DArray,
	const void* const p_element
);

// 在一个DArray的开头添加一个元素
/*
参数：
	p_DArray：要添加元素的DArray的指针
	p_element：被添加元素的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示添加成功
		非0表示添加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int push_front_to_DArray(
	DArray* const p_DArray,
	const void* const p_element
);

// 在一个DArray的指定位置插入一个元素
/*
参数：
	p_DArray：要插入元素的DArray的指针
	index：插入位置的索引
	p_element：被插入元素的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示插入成功
		非0表示插入失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int insert_to_DArray(
	DArray* const p_DArray,
	uintmax_t index,
	const void* const p_element
);

// 从一个DArray的末尾删除一个元素
/*
参数：
	p_DArray：要删除元素的DArray的指针
*/
void pop_back_from_DArray(
	DArray* const p_DArray
);

// 从一个DArray的开头删除一个元素
/*
参数：
	p_DArray：要删除元素的DArray的指针
*/
void pop_front_from_DArray(
	DArray* const p_DArray
);

// 从一个DArray的指定位置删除一个元素
/*
参数：
	p_DArray：要删除元素的DArray的指针
	index：被删除元素的索引
*/
void remove_from_DArray(
	DArray* const p_DArray,
	uintmax_t index
);


// 3-添加数组，删除子数组

// 将一个DArray追加到另一个DArray后面
/*
参数：
	p_target_DArray：要追加的DArray的指针
	p_source_DArray：所追加的DArray的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示追加成功
		非0表示追加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int pop_back_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray
);

// 将一个C数组追加到另一个DArray后面
/*
参数：
	p_target_DArray：要追加的DArray的指针
	p_source_std_arr：所追加的C数组的指针
	element_count：所追加的C数组的元素个数
返回值：
	一个表示函数执行状态的整数
	（
		为0表示追加成功
		非0表示追加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int pop_back_std_arr(
	DArray* const p_target_DArray,
	const void* const p_source_std_arr,
	uintmax_t element_count
);

// 将一个DArray追加到另一个DArray前面
/*
参数：
	p_target_DArray：要追加的DArray的指针
	p_source_DArray：所追加的DArray的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示追加成功
		非0表示追加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int pop_front_DArray(
	DArray* const p_target_DArray,
	const DArray* const p_source_DArray
);

// 将一个C数组追加到另一个DArray前面
/*
参数：
	p_target_DArray：要追加的DArray的指针
	p_source_std_arr：所追加的C数组的指针
	element_count：所追加的C数组的元素个数
返回值：
	一个表示函数执行状态的整数
	（
		为0表示追加成功
		非0表示追加失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int pop_front_std_arr(
	DArray* const p_target_DArray,
	const void* const p_source_std_arr,
	uintmax_t element_count
);

// 将一个DArray插入另一个DArray中的指定索引处
/*
参数：
	p_target_DArray：要插入的DArray的指针
	index：所插入位置的索引
	p_source_DArray：所插入的DArray的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示插入成功
		非0表示插入失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int insert_DArray(
	DArray* const p_target_DArray,
	uintmax_t index,
	const DArray* const p_source_DArray
);

// 将一个C数组插入另一个DArray中的指定索引处
/*
参数：
	p_target_DArray：要插入的DArray的指针
	index：所插入位置的索引
	p_source_std_arr：所插入的C数组的指针
	element_count：所插入的C数组的元素个数
返回值：
	一个表示函数执行状态的整数
	（
		为0表示插入成功
		非0表示插入失败（-1表示参数有误，-2表示分配内存空间失败）
	）
*/
int insert_std_arr(
	DArray* const p_target_DArray,
	uintmax_t index,
	const void* const p_source_std_arr,
	uintmax_t element_count
);

// 删除一个DArray的一部分
/*
参数：
	p_DArray：要删除的DArray的指针
	start_index：要删除部分的起始位置的索引
	end_index：要删除部分的结束位置的索引
*/
void remove_part_from_DArray(
	DArray* const p_DArray,
	uintmax_t start_index,
	uintmax_t end_index
);


// 4-获取元素

// 获取一个DArray中的第一个元素的指针
/*
参数：
	p_DArray：要获取元素的DArray的指针
返回值：
	指向所获取元素的指针
*/
void* first_of_DArray(
	const DArray* const p_DArray
);

// 获取一个DArray中的最后一个元素的指针
/*
参数：
	p_DArray：要获取元素的DArray的指针
返回值：
	指向所获取元素的指针
*/
void* last_of_DArray(
	const DArray* const p_DArray
);

// 获取一个DArray中指定位置的元素的指针
/*
参数：
	p_DArray：要获取元素的DArray的指针
	index：要获取元素的索引
返回值：
	指向所获取元素的指针
*/
void* index_of_DArray(
	const DArray* const p_DArray,
	uintmax_t index
);


// 5-修改元素

// 修改一个DArray中指定位置的元素的值
/*
参数：
	p_DArray：要修改元素的DArray的指针
	index：要修改元素的索引
	p_new_value：所修改的元素的新值的指针
返回值：
	一个表示函数执行状态的整数
	（
		为0表示修改成功
		非0表示修改失败
	）
*/
int modify_index_of_DArray(
	DArray* const p_DArray,
	uintmax_t index,
	const void* const p_new_value
);


// 6-查找元素

// 检查一个DArray中是否包含指定元素
/*
参数：
	p_DArray：要检查的DArray的指针
	p_element：要检查元素的指针
	comparator：比较两个元素的函数
	（
		该函数应：
		接受两个参数：两个const void * const指针
					用于接收所比较的两个元素
		返回一个整数：为0表示两元素相等
					非0表示两元素不相等
	）
返回值：
	一个布尔值
	（
		为true表示数组中存在相应元素
		为false则表示不存在
	）
*/
bool is_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void* const, const void* const)
);

// 检查一个元素在一个DArray中出现的次数
// 检查一个DArray中是否包含指定元素
/*
参数：
	p_DArray：要检查的DArray的指针
	p_element：要检查元素的指针
	comparator：比较两个元素的函数
	（
		该函数应：
		接受两个参数：两个const void * const指针
					用于接收所比较的两个元素
		返回一个整数：为0表示两元素相等
					非0表示两元素不相等
	）
返回值：
	一个无符号整数：表示所检查元素在数组中出现的次数
*/
uintmax_t num_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void* const, const void* const)
);

// 检查一个元素在一个DArray中出现的所有位置
// 并使用其索引值创建一个DArray返回
// 检查一个DArray中是否包含指定元素
/*
参数：
	p_DArray：要检查的DArray的指针
	p_element：要检查元素的指针
	comparator：比较两个元素的函数
	（
		该函数应：
		接受两个参数：两个const void * const指针
					用于接收所比较的两个元素
		返回一个整数：为0表示两元素相等
					非0表示两元素不相等
	）
返回值：
	一个DArray变量：其中保存了，所检查元素，在一个DArray中所有出现的索引
*/
DArray indexs_in_DArray(
	const DArray* const p_DArray,
	const void* const p_element,
	int(*comparator)(const void* const, const void* const)
);


// 7-排序数组

// 按照指定的比较函数，对一个DArray排序
/*
参数：
	p_DArray：要排序的DArray的指针
	is_order：是否顺序排序：true表示顺序，false表示倒序
	comparator：比较两个元素的函数
	（
		该函数应：
		接受两个参数：两个const void * const指针用于接收所比较的两个元素
		返回一个整数：
			为0表示两元素相等
			>0表示第一个元素>第二个元素
			<0表示第一个元素<第二个元素
	）
*/
void sort_DArray(
	DArray* const p_DArray,
	bool is_order,
	int(*comparator)(const void* const, const void* const)
);


// 8-反转数组

// 将一个DArray顺序颠倒
/*
参数：
	p_DArray：要翻转的DArray的指针
*/
void reverse_DArray(
	DArray* const p_DArray
);


// 9-数组属性

// 获取一个DArray中的元素个数
/*
参数：
	p_DArray：要获取的DArray的指针
返回值：
	一个无符号整数，表示所获取DArray的元素个数
*/
uintmax_t count_of_DArray(
	const DArray* const p_DArray
);

// 获取一个DArray单个元素的大小（单位：字节）
/*
参数：
	p_DArray：要获取的DArray的指针
返回值：
	一个无符号整数，表示所获取DArray的单个元素的大小
*/
size_t element_size_of_DArray(
	const DArray* const p_DArray
);

// 检查一个DArray是否为空
/*
参数：
	p_DArray：要检查的DArray的指针
返回值：
	一个布尔值，表示所检查DArray是否为空
*/
bool is_empty_DArray(
	const DArray* const p_DArray
);


// 10-遍历数组

// 对一个DArray中的每个元素执行指定的函数
/*
参数：
	p_DArray：要遍历的DArray的指针
	traversal：遍历每个元素的函数，该函数应：
			无返回值，使用一个void *形参来接收元素指针
*/
void traverse_DArray(
	const DArray* const p_DArray,
	void (*traversal)(void* const)
);


#endif