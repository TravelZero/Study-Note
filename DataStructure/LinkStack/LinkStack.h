#pragma once

#include<stdlib.h>
#include"LinkList.h"

struct StackNode
{
	struct StackNode *next;
};


struct LStack
{
	struct StackNode header; //ͷ���
	int size;
};

typedef void* LinkStack;

#ifdef __cplusplus
extern "C"{
#endif	

	//��ʼ��
	LinkStack Init_LinkStack();
	//��ջ
	void Push_LinkStack(LinkStack stack, void *data);
	//��ջ
	void Pop_LinkStack(LinkStack stack);
	//���ջ��Ԫ��
	void* Top_LinkStack(LinkStack stack);
	//��ô�С
	int Size_LinkStack(LinkStack stack);
	//����ջ
	void Destroy_LinkStack(LinkStack stack);

#ifdef __cplusplus
}
#endif	
