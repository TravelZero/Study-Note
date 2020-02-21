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

	//初始化链表
	LinkList Init_LinkList();
	//插入节点
	void Insert_LinkList(LinkList list,int pos,void *data);
	//遍历链表
	void Foreach_LinkList(LinkList list, FOREACH myforeach);
	//按位置删除
	void RemoveByPos_LinkList(LinkList list,int pos);
	//按值删除
	void RemoveByVal_LinkList(LinkList list, void *data, COMPARE compare);
	//清空链表
	void Clear_LinkList(LinkList list);
	//大小
	int Size_LinkList(LinkList list);
	//销毁链表
	void Destroy_LinkList(LinkList list);


#ifdef __cplusplus
}
#endif