#include"SeqStack.h"

//��ʼ��
SeqStack Init_SeqStack()
{
	struct SStack *stack = malloc(sizeof(struct SStack));
	if (NULL == stack)
	{
		return NULL;
	}

	//memset(stack, 0, sizeof(struct SStack));
	stack->size = 0;
	for (int i = 0; i < MAX; ++i)
	{
		stack->data[i] = NULL;
	}
	
	return stack;
}
//��ջ
void Push_SeqStack(SeqStack stack, void *data)
{

	if (NULL == stack)
	{
		return;
	}

	if (NULL == data)
	{
		return;
	}

	struct SStack *s = (struct SStack *)stack;

	if (s->size == MAX)
	{
		return;
	}

	s->data[s->size] = data;
	s->size++;
}
//��ջ
void Pop_SeqStack(SeqStack stack)
{

	if (NULL == stack)
	{
		return;
	}

	struct SStack *s = (struct SStack *)stack;

	if (s->size == 0)
	{
		return;
	}


	s->data[s->size - 1] = NULL;
	s->size--;
}
//���ջ��Ԫ��
void *Top_SeqStack(SeqStack stack)
{
	if (NULL == stack)
	{
		return NULL;
	}

	struct SStack *s = (struct SStack *)stack;

	if (s->size == 0)
	{
		return NULL;
	}


	return s->data[s->size - 1];

}
//���ջ�Ĵ�С
int Size_SeqStack(SeqStack stack)
{
	if (NULL == stack)
	{
		return -1;
	}

	struct SStack *s = (struct SStack *)stack;

	return s->size;
}
//����ջ
void Destroy_SeqStack(SeqStack stack)
{

	if (NULL == stack)
	{
		return;
	}

	free(stack);
}