#pragma once

struct BiNode
{
	int data;
	struct BiNode *lchild;
	struct BiNode *rchild;
};


#include<stdlib.h>

//���������������
struct QueueNode
{
	struct QueueNode *next;
};

//������������
struct LQueue
{
	struct QueueNode header; //ͷ���
	int size;
	struct QueueNode *rear; //βָ�룬ʼ��ָ����������һ���ڵ�
};

typedef void* LinkQueue;


#ifdef __cplusplus
extern "C"{
#endif

	//��ʼ��
	LinkQueue Init_LinkQueue();
	//���
	void Push_LinkQueue(LinkQueue queue, void *data);
	//����
	void Pop_LinkQueue(LinkQueue queue);
	//��ö�ͷԪ��
	void* Front_LinkQueue(LinkQueue queue);
	//��ö�βԪ��
	void* Back_LinkQueue(LinkQueue queue);
	//��С
	int Size_LinkQueue(LinkQueue queue);
	//���ٶ���
	void Destroy_LinkQueue(LinkQueue queue);

#ifdef __cplusplus
}
#endif