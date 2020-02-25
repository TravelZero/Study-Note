#include"LinkStack.h"


//��ʼ��
LinkStack Init_LinkStack()
{


	struct LStack *stack = malloc(sizeof(struct LStack));
	if (NULL == stack)
	{
		return NULL;
	}

	stack->header.next = NULL;
	stack->size = 0;

	return stack;

}
//��ջ
void Push_LinkStack(LinkStack stack, void *data)
{

	if (NULL == stack)
	{	
		return;
	}

	if (NULL == data)
	{
	
		return;
	}

	struct LStack *ls = (struct LStack *)stack;
	struct StackNode *node = (struct StackNode *)data;


	node->next = ls->header.next;
	ls->header.next = node;

	++(ls->size);

}
//��ջ
void Pop_LinkStack(LinkStack stack)
{
	if (NULL == stack)
	{
		return;
	}

	struct LStack *ls = (struct LStack *)stack;

	if (ls->size == 0)
	{
		return;
	}

	//�����µ�һ���ڵ�
	struct StackNode *pFirst = ls->header.next;

	ls->header.next = pFirst->next;

	ls->size--;
}
//���ջ��Ԫ��
void* Top_LinkStack(LinkStack stack)
{
	if (NULL == stack)
	{
		return NULL;
	}

	struct LStack *ls = (struct LStack *)stack;

	if (ls->size == 0)
	{
		return NULL;
	}


	return ls->header.next;
}
//��ô�С
int Size_LinkStack(LinkStack stack)
{
	if (NULL == stack)
	{
		return -1;
	}

	struct LStack *ls = (struct LStack *)stack;

	return ls->size;
}
//����ջ
void Destroy_LinkStack(LinkStack stack)
{

	if (NULL == stack)
	{
		return;
	}

	free(stack);
	stack = NULL;
}