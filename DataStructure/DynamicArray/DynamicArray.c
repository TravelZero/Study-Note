#include"DynamicArray.h"

//初始化数组
struct DynamicArray *Init_DynamicArray(int capacity)
{
	if (capacity <= 0)
	{
		return NULL;
	}

	struct DynamicArray *arr = malloc(sizeof(struct DynamicArray));
	if (NULL == arr)
	{
		return NULL;
	}
	arr->capacity = capacity;
	arr->addr = malloc(sizeof(void *)*arr->capacity);
	arr->size = 0;

	return arr;
}
//插入元素
void Insert_DynamicArray(struct DynamicArray *arr, int pos, void *data)
{

	if (NULL == arr)
	{
		return;
	}

	if (NULL == data)
	{
		return;
	}

	if (pos < 0 || pos > arr->size)
	{
		pos = arr->size;
	}

	//判断空间是否足够
	if (arr->size >= arr->capacity)
	{

		//1. 申请一块更大的内存空间
		int newcapacity = arr->capacity * 2;
		void **newspace = malloc(sizeof(void *)* newcapacity);

		//2. 将原来空间的数据拷贝到新空间
		memcpy(newspace, arr->addr, sizeof(void *)* arr->capacity);

		//3. 释放原来空间的内存
		free(arr->addr);

		//4. 更新addr指向
		arr->addr = newspace;
		arr->capacity = newcapacity;

	}


	//移动元素，给pos位置空出位置来
	for (int i = arr->size - 1; i >= pos; --i)
	{
		arr->addr[i + 1] = arr->addr[i];
	}

	//将新元素插入到pos位置
	arr->addr[pos] = data;
	arr->size++;
}
//遍历
void Foreach_DynamicArray(struct DynamicArray *arr, void(*_callback)(void *))
{
	if (NULL == arr)
	{
		return;
	}

	if (NULL == _callback)
	{
		return;
	}

	for (int i = 0; i < arr->size; ++i)
	{
		_callback(arr->addr[i]);
	}


}

//位置删除
void RemoveByPos_DynamicArray(struct DynamicArray *arr, int pos)
{

	if (NULL == arr)
	{
		return;
	}

	if (pos < 0 || pos > arr->size - 1)
	{
		return;
	}


	for (int i = pos; i < arr->size - 1; ++i)
	{
		arr->addr[i] = arr->addr[i + 1];
	}


	arr->size--;
}

//按值删除
void RemoveByValue_DynamicArray(struct DynamicArray *arr, void *data, int(*compare)(void*, void *))
{
	if (NULL == arr)
	{
		return;
	}

	if (NULL == data)
	{
		return;
	}

	if (NULL == compare)
	{
		return;
	}


	for (int i = 0; i < arr->size; ++i)
	{
		if (compare(arr->addr[i], data))
		{
			RemoveByPos_DynamicArray(arr, i);
			break;
		}
	}

}

//销毁数组
void Destroy_DynamicArray(struct DynamicArray *arr)
{
	if (NULL == arr)
	{
		return;
	}

	if (arr->addr != NULL)
	{
		free(arr->addr);
		arr->addr = NULL;
	}

	free(arr);
	arr = NULL;
}
