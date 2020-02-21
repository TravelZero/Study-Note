#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"DynamicArray.h"


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

int myCompare(void *d1,void *d2)
{
	struct Person *p1 = (struct Person *)d1;
	struct Person *p2 = (struct Person *)d2;

	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}


void test()
{
	//创建动态数组
	struct DynamicArray *da = Init_DynamicArray(5);
	//动态数组添加元素
	struct Person p1 = { "aaa", 10 };
	struct Person p2 = { "bbb", 20 };
	struct Person p3 = { "ccc", 30 };
	struct Person p4 = { "ddd", 40 };
	struct Person p5 = { "eee", 50 };
	struct Person p6 = { "fff", 60 };
	
	Insert_DynamicArray(da, 0, &p1);
	Insert_DynamicArray(da, 0, &p2);
	Insert_DynamicArray(da, 0, &p3);
	Insert_DynamicArray(da, 1, &p4);
	Insert_DynamicArray(da, 1, &p5);

	printf("capacity:%d\n",da->capacity);
	Insert_DynamicArray(da, 100, &p6);  //3 5 4 2 1 6
	printf("capacity:%d\n", da->capacity);

	Foreach_DynamicArray(da, myPrint);

	printf("---------------\n");
	RemoveByPos_DynamicArray(da,2);//3 5 2 1 6
	Foreach_DynamicArray(da, myPrint);

	printf("---------------\n");
	struct Person pDel = { "aaa", 10 };
	RemoveByValue_DynamicArray(da, &pDel, myCompare);

	//da->addr = NULL;

	Foreach_DynamicArray(da, myPrint);

	//销毁
	Destroy_DynamicArray(da);
}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}