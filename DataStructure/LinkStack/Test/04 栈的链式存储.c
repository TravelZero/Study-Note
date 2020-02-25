#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"LinkStack.h"

struct Person
{
	struct StackNode node;
	char name[64];
	int age;
};

void test()
{

	//��ʼ��ջ
	LinkStack stack = Init_LinkStack();
	
	//��������
	struct Person p1 = { NULL, "aaa", 10 };
	struct Person p2 = { NULL, "bbb", 20 };
	struct Person p3 = { NULL, "ccc", 30 };
	struct Person p4 = { NULL, "ddd", 40 };
	struct Person p5 = { NULL, "eee", 50 };
	struct Person p6 = { NULL, "fff", 60 };

	//������ջ
	Push_LinkStack(stack, &p1);
	Push_LinkStack(stack, &p2);
	Push_LinkStack(stack, &p3);
	Push_LinkStack(stack, &p4);
	Push_LinkStack(stack, &p5);
	Push_LinkStack(stack, &p6);

	//���ջ������Ԫ��
	while (Size_LinkStack(stack) > 0)
	{

		//���ջ��Ԫ��
		struct Person *person = (struct Person *)Top_LinkStack(stack);
		//��ӡ
		printf("Name:%s Age:%d\n", person->name, person->age);
		//����ջ��Ԫ��
		Pop_LinkStack(stack);

	}

	printf("Size:%d\n", Size_LinkStack(stack));

}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}