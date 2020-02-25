#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#ifdef __cplusplus
extern "C"{
#endif

	//1. �Ȱ�����Ҫ��������Ϣ�ṹ��������
	struct DynamicArray
	{
		//����洢Ԫ�صĿռ���׵�ַ
		void **addr;
		//�洢���ݵ��ڴ�������ܹ����ɶ���Ԫ��
		int capacity; //����
		//��ǰ�洢���ݵ��ڴ����ж��ٸ�Ԫ��
		int size; //��С
	};


	//��ʼ������
	struct DynamicArray *Init_DynamicArray(int capacity);
	//����Ԫ��
	void Insert_DynamicArray(struct DynamicArray *arr, int pos, void *data);
	//����
	void Foreach_DynamicArray(struct DynamicArray *arr, void(*_callback)(void *));
	//λ��ɾ��
	void RemoveByPos_DynamicArray(struct DynamicArray *arr, int pos);
	//��ֵɾ��
	void RemoveByValue_DynamicArray(struct DynamicArray *arr, void *data, int(*compare)(void*, void *));
	//��������
	void Destroy_DynamicArray(struct DynamicArray *arr);

#ifdef __cplusplus
}
#endif