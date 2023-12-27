#include "Dynamic_Array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 宏定义函数（返回值为整数）返回值
// 参数相关错误
#define PARAMETER_ERROR -1
// 内存相关错误
#define MEMORY_ERROR -2
// 函数执行成功
#define SUCCESS 0


// 静态函数，用于创建一个初始化状态
// （data为NULL，count和type_size为0）的DArray变量
static DArray create_empty_DArray(void);

// 静态函数，用于调整内存空间大小
/*
参数：
    origin_data，原内存空间地址
    new_m_count，新内存空间长度（以new_m_size为单位）
    new_m_size，新内存空间单位大小（以字节为单位）
返回值：
    新内存空间地址
*/
static void* resize_memory(
    void* const origin_data,
    uintmax_t new_m_count,
    size_t new_m_size
);

// 静态函数，用于内存数据的拷贝
/*
参数：
    target_memory，拷贝的目标的起始地址
    source_memory，拷贝的源的起始地址
    copy_size，拷贝的大小（单位：字节）
*/
static void copy_memory(
    void* const target_memory,
    const void* const source_memory,
    size_t copy_size
);

// 静态函数，用于将一些内存数据插入一个插入一段内存
/*
参数：
    target_data:要插入内存段的起始地址
    insert_index：插入的位置索引
    type_size：数据单位内存大小
    origin_count：要插入内存段的起始数据个数
    source_memory，所插入数据的起始地址
    insert_count，所插入数据的数量
*/
static void insert_data(
    void* const target_data,
    uintmax_t insert_index,
    size_t type_size,
    uintmax_t origin_count,
    const void* const source_data,
    uintmax_t insert_count
);

// 静态函数，用于从一段内存中删除一些数据
/*
参数：
    target_data:要删除内存段的起始地址
    remove_index：删除的起始位置索引
    type_size：数据单位内存大小
    origin_count：要删除内存段的起始数据个数
    remove_count：要删除数据的个数
*/
static void remove_data(
    void* const target_data,
    uintmax_t remove_index,
    size_t type_size,
    uintmax_t origin_count,
    uintmax_t remove_count
);

// 静态函数，用于交换两块内存空间的数据
/*
参数：
    data_1：第一块内存空间的指针
    data_2：第二块内存空间的指针
    type_size：所交换内存空间的大小
*/
static void exchange_memory(
    void* data_1,
    void* data_2,
    size_t type_size
);

// 1-创建、清理

// 创建一个空的，指定数组类型大小的DArray
DArray create_DArray(
    size_t type_size
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理
    // 则直接返回初始化状态的空DArray
    DArray new_DArray = create_empty_DArray();
    if (type_size == 0) return new_DArray;

    // 根据传入参数修改new_DArray的成员变量
    new_DArray.type_size = type_size;

    return new_DArray;
}

// 复制一个C数组，来创建一个DArray
DArray copy_from_std_arr(
    const void* const p_std_arr,
    uintmax_t element_count,
    size_t element_size
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理
    // 则直接返回初始化状态的空DArray
    DArray new_DArray = create_empty_DArray();
    if (p_std_arr == NULL || element_count == 0 || element_size == 0)
        return new_DArray;

    // 为new_DArray分配内存空间
    void* new_data = resize_memory(NULL, element_count, element_size);

    // 如果内存空间分配失败，直接返回初始化状态的空DArray
    if (new_data == NULL) return new_DArray;

    // 将C数组中的数据拷贝到新数组中
    insert_data(new_data, 0, element_size, 0, p_std_arr, element_count);

    // 修改new_DArray成员变量的值
    new_DArray.data = new_data;
    new_DArray.type_size = element_size;
    new_DArray.count = element_count;

    return new_DArray;
}

// 复制一个DArray，来创建一个DArray
DArray copy_from_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理
    // 则直接返回初始化状态的空DArray
    DArray new_DArray = create_empty_DArray();
    if (p_DArray == NULL) return new_DArray;

    // 为new_DArray分配内存空间
    void* new_data = resize_memory(NULL, p_DArray->count, p_DArray->type_size);

    // 如果内存空间分配失败，直接返回初始化状态的空DArray
    if (new_data == NULL) return new_DArray;

    // 将源数组中的数据拷贝到新数组中
    insert_data(new_data, 0, p_DArray->type_size, 0, p_DArray->data, p_DArray->count);

    // 修改new_DArray成员变量的值
    new_DArray.data = new_data;
    new_DArray.type_size = p_DArray->type_size;
    new_DArray.count = p_DArray->count;

    return new_DArray;
}

// 复制一个C数组中的一部分，来创建一个DArray
DArray copy_part_from_std_arr(
    const void* const p_std_arr,
    size_t element_size,
    uintmax_t start_index,
    uintmax_t end_index
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理
    // 则直接返回初始化状态的空DArray
    DArray new_DArray = create_empty_DArray();
    if (p_std_arr == NULL || element_size == 0 || start_index > end_index)
        return new_DArray;

    // 为new_DArray分配内存空间
    void* new_data = resize_memory(NULL, end_index - start_index + 1, element_size);

    // 如果内存空间分配失败，直接返回初始化状态的空DArray
    if (new_data == NULL) return new_DArray;

    // 将源数组中的数据拷贝到新数组中
    insert_data(new_data, 0, element_size, 0, (char*)p_std_arr + start_index * element_size,
        end_index - start_index + 1);

    // 修改new_DArray成员变量的值
    new_DArray.data = new_data;
    new_DArray.type_size = element_size;
    new_DArray.count = end_index - start_index + 1;

    return new_DArray;
}

// 复制一个DArray中的一部分，来创建一个DArray
DArray copy_part_from_DArray(
    const DArray* const p_DArray,
    uintmax_t start_index,
    uintmax_t end_index
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理
    // 则直接返回初始化状态的空DArray
    DArray new_DArray = create_empty_DArray();
    if (p_DArray == NULL || start_index > end_index || end_index >= p_DArray->count)
        return new_DArray;

    // 为new_DArray分配内存空间
    void* new_data = resize_memory(NULL, end_index - start_index + 1, p_DArray->type_size);

    // 如果内存空间分配失败，直接返回初始化状态的空DArray
    if (new_data == NULL) return new_DArray;

    // 将源数组中的数据拷贝到新数组中
    insert_data(new_data, 0, p_DArray->type_size, 0,
        p_DArray->data + start_index * p_DArray->type_size,
        end_index - start_index + 1);

    // 修改new_DArray成员变量的值
    new_DArray.data = new_data;
    new_DArray.type_size = p_DArray->type_size;
    new_DArray.count = end_index - start_index + 1;

    return new_DArray;
}

// 清空一个DArray
void clear_DArray(
    DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL) return;

    // 将DArray动态分配的内存空间释放
    resize_memory(p_DArray->data, 0, 0);

    // 修改DArray的成员变量的值
    p_DArray->data = NULL;
    p_DArray->count = 0;
}

// 2-添加、删除元素

// 在一个DArray的末尾添加一个元素
int push_back_to_DArray(
    DArray* const p_DArray,
    const void* const p_element
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_DArray == NULL || p_element == NULL || p_DArray->type_size == 0)
        return PARAMETER_ERROR;

    // 调整DArray的内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count + 1, p_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 拷贝所添加元素数据到数组中
    insert_data(new_data, p_DArray->count, p_DArray->type_size, p_DArray->count, p_element, 1);

    // 修改DArray的成员变量的值
    p_DArray->data = new_data;
    p_DArray->count++;

    return SUCCESS;
}

// 在一个DArray的开头添加一个元素
int push_front_to_DArray(
    DArray* const p_DArray,
    const void* const p_element
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_DArray == NULL || p_element == NULL || p_DArray->type_size == 0)
        return PARAMETER_ERROR;

    // 调整DArray的内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count + 1, p_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 拷贝所添加元素数据到数组中
    insert_data(new_data, 0, p_DArray->type_size, p_DArray->count, p_element, 1);

    // 修改DArray的成员变量的值
    p_DArray->data = new_data;
    p_DArray->count++;

    return SUCCESS;
}


// 在一个DArray的指定位置插入一个元素
int insert_to_DArray(
    DArray* const p_DArray,
    uintmax_t index,
    const void* const p_element
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_DArray == NULL || p_element == NULL || p_DArray->type_size == 0 ||
        index > p_DArray->count)
        return PARAMETER_ERROR;

    // 调整DArray的内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count + 1, p_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 拷贝所添加元素数据到数组中
    insert_data(new_data, index, p_DArray->type_size, p_DArray->count, p_element, 1);

    // 修改DArray的成员变量的值
    p_DArray->data = new_data;
    p_DArray->count++;

    return SUCCESS;
}


// 从一个DArray的末尾删除一个元素
void pop_back_from_DArray(
    DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || p_DArray->type_size == 0 || p_DArray->data == NULL ||
        p_DArray->count == 0) return;

    // 直接调整数组内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count - 1, p_DArray->type_size);

    // 如果调整出现错误，则直接返回
    if (new_data == NULL) return;

    // 修改DArray成员变量的值
    p_DArray->data = new_data;
    p_DArray->count--;
}


// 从一个DArray的开头删除一个元素
void pop_front_from_DArray(
    DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || p_DArray->type_size == 0 || p_DArray->data == NULL ||
        p_DArray->count == 0) return;

    // 删除开头元素
    remove_data(p_DArray->data, 0, p_DArray->type_size, p_DArray->count, 1);

    // 调整数组内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count - 1, p_DArray->type_size);

    // 如果调整出现错误，则直接返回
    if (new_data == NULL) return;

    // 修改DArray成员变量的值
    p_DArray->data = new_data;
    p_DArray->count--;
}

// 从一个DArray的指定位置删除一个元素
void remove_from_DArray(
    DArray* const p_DArray,
    uintmax_t index
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || p_DArray->type_size == 0 || p_DArray->data == NULL ||
        index >= p_DArray->count) return;

    // 删除指定元素
    remove_data(p_DArray->data, index, p_DArray->type_size, p_DArray->count, 1);

    // 调整数组内存空间
    void* new_data = resize_memory(p_DArray->data, p_DArray->count - 1, p_DArray->type_size);

    // 如果调整出现错误，则直接返回
    if (new_data == NULL) return;

    // 修改DArray成员变量的值
    p_DArray->data = new_data;
    p_DArray->count--;
}


// 3-添加数组，删除子数组

// 将一个DArray追加到另一个DArray后面
int pop_back_DArray(
    DArray* const p_target_DArray,
    const DArray* const p_source_DArray
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_DArray == NULL || p_target_DArray->type_size == 0 ||
        p_target_DArray->type_size != p_source_DArray->type_size || p_source_DArray->data == NULL ||
        p_source_DArray->count == 0) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + p_source_DArray->count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, p_target_DArray->count, p_target_DArray->type_size, p_target_DArray->count,
        p_source_DArray->data, p_source_DArray->count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += p_source_DArray->count;

    return SUCCESS;
}


// 将一个C数组追加到另一个DArray后面
int pop_back_std_arr(
    DArray* const p_target_DArray,
    const void* const p_source_std_arr,
    uintmax_t element_count
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_std_arr == NULL || p_target_DArray->type_size == 0 ||
        element_count == 0) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + element_count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, p_target_DArray->count, p_target_DArray->type_size, p_target_DArray->count,
        p_source_std_arr, element_count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += element_count;

    return SUCCESS;
}


// 将一个DArray追加到另一个DArray前面
int pop_front_DArray(
    DArray* const p_target_DArray,
    const DArray* const p_source_DArray
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_DArray == NULL || p_target_DArray->type_size == 0 ||
        p_target_DArray->type_size != p_source_DArray->type_size || p_source_DArray->data == NULL ||
        p_source_DArray->count == 0) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + p_source_DArray->count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, 0, p_target_DArray->type_size, p_target_DArray->count,
        p_source_DArray->data, p_source_DArray->count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += p_source_DArray->count;

    return SUCCESS;
}


// 将一个C数组链接到另一个DArray前面
int pop_front_std_arr(
    DArray* const p_target_DArray,
    const void* const p_source_std_arr,
    uintmax_t element_count
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_std_arr == NULL || p_target_DArray->type_size == 0 ||
        element_count == 0) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + element_count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, p_target_DArray->count, p_target_DArray->type_size, p_target_DArray->count,
        p_source_std_arr, element_count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += element_count;

    return SUCCESS;
}


// 将一个DArray插入另一个DArray中的指定索引处
int insert_DArray(
    DArray* const p_target_DArray,
    uintmax_t index,
    const DArray* const p_source_DArray
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_DArray == NULL || p_target_DArray->type_size == 0 ||
        p_target_DArray->type_size != p_source_DArray->type_size || p_source_DArray->data == NULL ||
        p_source_DArray->count == 0 || index > p_target_DArray->count) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + p_source_DArray->count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, index, p_target_DArray->type_size, p_target_DArray->count,
        p_source_DArray->data, p_source_DArray->count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += p_source_DArray->count;

    return SUCCESS;
}


// 将一个C数组插入另一个DArray中的指定索引处
int insert_std_arr(
    DArray* const p_target_DArray,
    uintmax_t index,
    const void* const p_source_std_arr,
    uintmax_t element_count
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_target_DArray == NULL || p_source_std_arr == NULL || p_target_DArray->type_size == 0 ||
        element_count == 0 || index > p_target_DArray->count) return PARAMETER_ERROR;

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_target_DArray->data, p_target_DArray->count + element_count,
        p_target_DArray->type_size);

    // 如果调整失败，则返回MEMORY_ERROR
    if (new_data == NULL) return MEMORY_ERROR;

    // 追加DArray
    insert_data(new_data, index, p_target_DArray->type_size, p_target_DArray->count,
        p_source_std_arr, element_count);

    // 修改DArray成员变量的值
    p_target_DArray->data = new_data;
    p_target_DArray->count += element_count;

    return SUCCESS;
}

// 删除一个DArray的一部分
void remove_part_from_DArray(
    DArray* const p_DArray,
    uintmax_t start_index,
    uintmax_t end_index
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0 || end_index >= p_DArray->count || end_index > start_index)
        return;

    // 删除指定的部分
    remove_data(p_DArray->data, start_index, p_DArray->type_size, p_DArray->count,
        end_index - start_index + 1);

    // 调整DArray内存空间大小
    void* new_data = resize_memory(p_DArray->data, p_DArray->count - (end_index - start_index + 1),
        p_DArray->type_size);

    // 如果调整失败，则直接返回
    if (new_data == NULL) return;

    // 修改DArray成员变量的值
    p_DArray->data = new_data;
    p_DArray->count -= end_index - start_index + 1;
}


// 4-获取元素

// 获取一个DArray中的第一个元素的指针
void* first_of_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则返回NULL
    if (p_DArray == NULL || p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0) return NULL;

    return p_DArray->data;
}


// 获取一个DArray中的最后一个元素的指针
void* last_of_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则返回NULL
    if (p_DArray == NULL || p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0) return NULL;

    return p_DArray->data + (p_DArray->count - 1) * p_DArray->type_size;
}


// 获取一个DArray中指定位置的元素的指针
void* index_of_DArray(
    const DArray* const p_DArray,
    uintmax_t index
)
{
    // 参数检查，如果参数不合理，则返回NULL
    if (p_DArray == NULL || p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0 || index >= p_DArray->count) return NULL;

    return p_DArray->data + index * p_DArray->type_size;
}


// 5-修改元素

// 修改一个DArray中指定位置的元素的值
int modify_index_of_DArray(
    DArray* const p_DArray,
    uintmax_t index,
    const void* const p_new_value
)
{
    // 参数检查，如果参数不合理，则返回PARAMETER_ERROR
    if (p_DArray == NULL || p_new_value == NULL || p_DArray->data == NULL ||
        p_DArray->type_size == 0 || p_DArray->count == 0 || index >= p_DArray->count)
        return PARAMETER_ERROR;

    // 拷贝新数据到所修改内存
    insert_data(p_DArray->data, index, p_DArray->type_size, index, p_new_value, 1);

    return SUCCESS;
}


// 6-查找元素

// 检查一个DArray中是否包含指定元素
bool is_in_DArray(
    const DArray* const p_DArray,
    const void* const p_element,
    int(*comparator)(const void* const, const void* const)
)
{
    // 参数检查，如果参数不合理，则返回false
    if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
        p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0)
        return false;

    // 遍历整个DArray，一旦遇到与所检查元素相同的元素，则返回true
    uintmax_t index = 0;
    while (index < p_DArray->count) {

        if (comparator(p_DArray->data + index * p_DArray->type_size, p_element) == 0)
            return true;

        index++;
    }

    // 遍历完整个DArray都没找到，则返回false
    return false;
}


// 检查一个元素在一个DArray中出现的次数
uintmax_t num_in_DArray(
    const DArray* const p_DArray,
    const void* const p_element,
    int(*comparator)(const void* const, const void* const)
)
{
    // 参数检查，如果参数不合理，则返回0
    if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
        p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0)
        return 0;

    // 循环统计与所查找元素相同的元素个数
    uintmax_t index = 0;
    uintmax_t num = 0;
    while (index < p_DArray->count) {
        if (comparator(p_DArray->data + index * p_DArray->type_size, p_element) == 0)
            num++;

        index++;
    }

    return num;
}


// 检查一个元素在一个DArray中出现的所有位置
//并将其索引值作为一个DArray返回
DArray indexs_in_DArray(
    const DArray* const p_DArray,
    const void* const p_element,
    int(*comparator)(const void* const, const void* const)
)
{
    // 参数检查，先创建一个初始化状态的空DArray，如果参数不合理，则返回空的DArray
    DArray indexs = create_empty_DArray();
    if (p_DArray == NULL || p_element == NULL || comparator == NULL ||
        p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count == 0)
        return indexs;

    // 创建一个uintmax_t类型的DArray
    indexs = create_DArray(sizeof(uintmax_t));

    // 循环，记录所有与所查找元素相同的元素的索引
    uintmax_t index = 0;
    while (index < p_DArray->count) {
        if (comparator(p_DArray->data + index * p_DArray->type_size, p_element) == 0)
            push_back_to_DArray(&indexs, &index);

        index++;
    }

    return indexs;
}


// 7-排序数组

// 按照指定的比较函数，对一个DArray排序
void sort_DArray(
    DArray* const p_DArray,
    bool is_order,
    int(*comparator)(const void* const, const void* const)
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || comparator == NULL || p_DArray->data == NULL ||
        p_DArray->type_size == 0 || p_DArray->count <= 1) return;

    // 排序
    uintmax_t count = 0;
    uintmax_t index = 0;

    while (count < p_DArray->count - 1) {
        index = 0;
        while (index < p_DArray->count - 1 - count) {
            void* data_1 = p_DArray->data + (index * p_DArray->type_size);
            void* data_2 = p_DArray->data + ((index + 1) * p_DArray->type_size);

            int cmp = comparator(data_1, data_2);

            if (is_order) {
                if (cmp > 0) {
                    exchange_memory(data_1, data_2, p_DArray->type_size);
                }
            }
            else {
                if (cmp < 0) {
                    exchange_memory(data_1, data_2, p_DArray->type_size);
                }
            }
            index++;
        }
        count++;
    }
}


// 8-反转数组

// 将一个DArray顺序颠倒
void reverse_DArray(
    DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || p_DArray->data == NULL || p_DArray->type_size == 0 ||
        p_DArray->count <= 1) return;

    // 反转数组
    uintmax_t index = 0;
    while (index < p_DArray->count / 2) {

        exchange_memory(p_DArray->data + (index * p_DArray->type_size),
            p_DArray->data + ((p_DArray->count - 1 - index) * p_DArray->type_size),
            p_DArray->type_size);

        index++;
    }
}


// 9-数组属性

// 获取一个DArray中的元素个数
uintmax_t count_of_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则返回0
    if (p_DArray == NULL) return 0;

    return p_DArray->count;
}


// 获取一个DArray单个元素的大小（单位：字节）
size_t element_size_of_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则返回0
    if (p_DArray == NULL) return 0;

    return p_DArray->type_size;
}


// 检查一个DArray是否为空
bool is_empty_DArray(
    const DArray* const p_DArray
)
{
    // 参数检查，如果参数不合理，则返回true
    if (p_DArray == NULL) return true;

    if (p_DArray->count == 0) return true;
    else return false;
}


// 10-遍历数组

// 对一个DArray中的每个元素执行指定的函数
void traverse_DArray(
    const DArray* const p_DArray,
    void (*traversal)(void* const)
)
{
    // 参数检查，如果参数不合理，则直接返回
    if (p_DArray == NULL || traversal == NULL) return;

    if (p_DArray->data == NULL ||
        p_DArray->type_size == 0 ||
        p_DArray->count == 0) return;

    // 遍历数组
    uintmax_t index = 0;
    while (index < p_DArray->count) {
        traversal(p_DArray->data + (index * p_DArray->type_size));
        index++;
    }
}


// 静态函数

// 静态函数，用于创建一个初始化状态
// （data为NULL，count和type_size为0）的DArray变量 
static DArray create_empty_DArray(void) {

    DArray empty_DArray = { NULL, 0, 0 };

    return empty_DArray;
}

// 静态函数，用于调整内存空间大小
static void* resize_memory(
    void* origin_data,
    uintmax_t new_m_count,
    size_t new_m_size
)
{
    return realloc(origin_data, new_m_count * new_m_size);
}

// 静态函数，用于内存数据的拷贝
static void copy_memory(
    void* const target_memory,
    const void* const source_memory,
    size_t copy_size
)
{
    memmove(target_memory, source_memory, copy_size);
}

// 静态函数，用于将一些内存数据插入一段内存
static void insert_data(
    void* const target_data,
    uintmax_t insert_index,
    size_t type_size,
    uintmax_t origin_count,
    const void* const source_data,
    uintmax_t insert_count
)
{
    // 将插入位置及后面的原始数据后移
    copy_memory((char*)target_data + (insert_index + insert_count) * type_size,
        (char*)target_data + insert_index * type_size,
        (origin_count - insert_index) * type_size);

    // 拷贝所插入数据到指定索引处
    copy_memory((char*)target_data + insert_index * type_size,
        source_data, insert_count * type_size);
}

// 静态函数，用于从一段内存中删除一些数据
static void remove_data(
    void* const target_data,
    uintmax_t remove_index,
    size_t type_size,
    uintmax_t origin_count,
    uintmax_t remove_count
)
{
    // 拷贝要删除数据后的数据到要删除数据的起始位置（覆盖将要删除的数据）
    copy_memory((char*)target_data + remove_index * type_size,
        (char*)target_data + (remove_index + remove_count) * type_size,
        (origin_count - remove_index - remove_count) * type_size);
}

// 静态函数，用于交换两块内存空间的数据
static void exchange_memory(
    void* data_1,
    void* data_2,
    size_t type_size
)
{
    void* temp_data = malloc(type_size);
    if (temp_data == NULL) return;

    memmove(temp_data, data_1, type_size);

    memmove(data_1, data_2, type_size);

    memmove(data_2, temp_data, type_size);

    free(temp_data);
}
