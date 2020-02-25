#pragma once

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

	typedef void * LinkList;
	typedef void(*FOREACH)(void *);
	typedef int(*COMPARE)(void *,void *);

	//��ʼ������
	LinkList Init_LinkList();
	//����ڵ�
	void Insert_LinkList(LinkList list,int pos,void *data);
	//��������
	void Foreach_LinkList(LinkList list, FOREACH myforeach);
	//��λ��ɾ��
	void RemoveByPos_LinkList(LinkList list,int pos);
	//��ֵɾ��
	void RemoveByVal_LinkList(LinkList list, void *data, COMPARE compare);
	//�������
	void Clear_LinkList(LinkList list);
	//��С
	int Size_LinkList(LinkList list);
	//��������
	void Destroy_LinkList(LinkList list);


#ifdef __cplusplus
}
#endif