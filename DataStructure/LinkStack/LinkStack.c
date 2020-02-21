#include"LinkStack.h"


//初始化
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
//入栈
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
//出栈
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

	//缓存下第一个节点
	struct StackNode *pFirst = ls->header.next;

	ls->header.next = pFirst->next;

	ls->size--;
}
//获得栈顶元素
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
//获得大小
int Size_LinkStack(LinkStack stack)
{
	if (NULL == stack)
	{
		return -1;
	}

	struct LStack *ls = (struct LStack *)stack;

	return ls->size;
}
//销毁栈
void Destroy_LinkStack(LinkStack stack)
{

	if (NULL == stack)
	{
		return;
	}

	free(stack);
	stack = NULL;
}