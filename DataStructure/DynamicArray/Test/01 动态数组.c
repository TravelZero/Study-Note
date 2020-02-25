#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1. �Ȱ�����Ҫ��������Ϣ�ṹ��������
struct DynamicArray
{
	//����洢Ԫ�صĿռ���׵�ַ
	void **addr;
	//�洢���ݵ��ڴ�������ܹ����ɶ���Ԫ��
	int capacity; //����
	//��ǰ�洢���ݵ��ڴ����ж��ٸ�Ԫ��
	int size; //��С
};


//��ʼ������
struct DynamicArray *Init_DynamicArray(int capacity)
{
	if (capacity <=0 )
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
//����Ԫ��
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

	//�жϿռ��Ƿ��㹻
	if (arr->size >= arr->capacity)
	{

		//1. ����һ�������ڴ�ռ�
		int newcapacity = arr->capacity * 2;
		void **newspace = malloc(sizeof(void *)* newcapacity);

		//2. ��ԭ���ռ�����ݿ������¿ռ�
		memcpy(newspace, arr->addr, sizeof(void *) * arr->capacity);

		//3. �ͷ�ԭ���ռ���ڴ�
		free(arr->addr);
		
		//4. ����addrָ��
		arr->addr = newspace;
		arr->capacity = newcapacity;

	}


	//�ƶ�Ԫ�أ���posλ�ÿճ�λ����
	for (int i = arr->size - 1; i >= pos;--i)
	{
		arr->addr[i + 1] = arr->addr[i];
	}

	//����Ԫ�ز��뵽posλ��
	arr->addr[pos] = data;
	arr->size++;
}
//����
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


struct Person
{
	char name[64];
	int age;
};

void myPrint(void *data)
{
	struct Person *person = (struct Person *)data;
	printf("Name:%s Age:%d\n", person->name,person->age);
}



void test()
{
	//������̬����
	struct DynamicArray *da = Init_DynamicArray(5);
	//��̬�������Ԫ��
	struct Person p1 = { "aaa", 10 };
	struct Person p2 = { "bbb", 20 };
	struct Person p3 = { "ccc", 30 };
	struct Person p4 = { "ddd", 40 };
	struct Person p5 = { "eee", 50 };
	struct Person p6 = { "fff", 60 };

	
	Insert_DynamicArray(da, 100, &p1);
	Insert_DynamicArray(da, 100, &p2);
	Insert_DynamicArray(da, 100, &p3);
	Insert_DynamicArray(da, 100, &p4);
	Insert_DynamicArray(da, 100, &p5);

	printf("capacity:%d\n",da->capacity);
	Insert_DynamicArray(da, 100, &p6);
	printf("capacity:%d\n", da->capacity);


	Foreach_DynamicArray(da, myPrint);
}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}