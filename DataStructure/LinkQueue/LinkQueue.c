#include"LinkQueue.h"


//��ʼ��
LinkQueue Init_LinkQueue()
{
	struct LQueue *queue = malloc(sizeof(struct LQueue));
	if (NULL == queue)
	{
		return NULL;
	}

	queue->header.next = NULL;
	queue->size = 0;
	queue->rear = &(queue->header);

	return queue;
}
//���
void Push_LinkQueue(LinkQueue queue, void *data)
{
	if (NULL == queue)
	{
		return;
	}

	if (NULL == data)
	{
		return;
	}


	struct LQueue *q = (struct LQueue *)queue;
	struct QueueNode *n = (struct QueueNode *)data;

	q->rear->next = n;	//����βָ�����һ��ָ��n
	n->next = NULL;
	//����βָ��
	q->rear = n;

	q->size++;

}
//����
void Pop_LinkQueue(LinkQueue queue)
{
	if(NULL == queue)
	{
		return;
	}


	struct LQueue *q = (struct LQueue *)queue;
	
	if (q->size == 0)
	{
		return;
	}

	if (q->size == 1)
	{

		q->header.next = NULL;
		q->rear = &(q->header);
		q->size--;

		return;
	}

	struct QueueNode *pFirstNode = q->header.next;

	q->header.next = pFirstNode->next;

	q->size--;

}
//��ö�ͷԪ��
void* Front_LinkQueue(LinkQueue queue)
{
	if (NULL == queue)
	{
		return NULL;
	}

	struct LQueue *q = (struct LQueue *)queue;

	return q->header.next;
}
//��ö�βԪ��
void* Back_LinkQueue(LinkQueue queue)
{
	if (NULL == queue)
	{
		return NULL;
	}

	struct LQueue *q = (struct LQueue *)queue;

	return q->rear;
}
//��С
int Size_LinkQueue(LinkQueue queue)
{
	if (NULL == queue)
	{
		return -1;
	}

	struct LQueue *q = (struct LQueue *)queue;

	return q->size;
}
//���ٶ���
void Destroy_LinkQueue(LinkQueue queue)
{
	if (NULL == queue)
	{
		return;
	}

	struct LQueue *q = (struct LQueue *)queue;
	q->header.next = NULL;
	q->rear = NULL;
	q->size = 0;

	free(queue);
	queue = NULL;
}