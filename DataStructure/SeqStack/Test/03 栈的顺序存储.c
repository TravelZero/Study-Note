#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"SeqStack.h"

struct Person
{
	char name[64];
	int age;
};


void test()
{
	
	//��ʼ��ջ
	SeqStack stack = Init_SeqStack();


	//��������
	struct Person p1 = { "aaa", 10 };
	struct Person p2 = { "bbb", 20 };
	struct Person p3 = { "ccc", 30 };
	struct Person p4 = { "ddd", 40 };
	struct Person p5 = { "eee", 50 };
	struct Person p6 = { "fff", 60 };

	//������ջ
	Push_SeqStack(stack, &p1);
	Push_SeqStack(stack, &p2);
	Push_SeqStack(stack, &p3);
	Push_SeqStack(stack, &p4);
	Push_SeqStack(stack, &p5);
	Push_SeqStack(stack, &p6);

	//���ջ������Ԫ��
	while (Size_SeqStack(stack) > 0)
	{

		//���ջ��Ԫ��
		struct Person *person =  (struct Person *)Top_SeqStack(stack);
		//��ӡ
		printf("Name:%s Age:%d\n",person->name,person->age);
		//����ջ��Ԫ��
		Pop_SeqStack(stack);

	}

	printf("Size:%d\n", Size_SeqStack(stack));

	//����ջ
	Destroy_SeqStack(stack);
	stack = NULL;

}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}