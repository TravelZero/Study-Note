#pragma once

struct BiNode
{
	int data;
	struct BiNode *lchild;
	struct BiNode *rchild;
};


#include<stdlib.h>

//链表结点的数据类型
struct QueueNode
{
	struct QueueNode *next;
};

//链表数据类型
struct LQueue
{
	struct QueueNode header; //头结点
	int size;
	struct QueueNode *rear; //尾指针，始终指向链表的最后一个节点
};

typedef void* LinkQueue;


#ifdef __cplusplus
extern "C"{
#endif

	//初始化
	LinkQueue Init_LinkQueue();
	//入队
	void Push_LinkQueue(LinkQueue queue, void *data);
	//出队
	void Pop_LinkQueue(LinkQueue queue);
	//获得队头元素
	void* Front_LinkQueue(LinkQueue queue);
	//获得队尾元素
	void* Back_LinkQueue(LinkQueue queue);
	//大小
	int Size_LinkQueue(LinkQueue queue);
	//销毁队列
	void Destroy_LinkQueue(LinkQueue queue);

#ifdef __cplusplus
}
#endif