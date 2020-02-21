#ifndef DATA_SRUCTURE_LIST_INCLUDED
#define DATA_SRUCTURE_LIST_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif 

#include "DataType.h"

#define MAXSIZE 20      /*存储空间初始分配量*/

typedef int ElemType;   /*此类型可根据实际情况而定*/
typedef struct 
{
    ElemType data[MAXSIZE];
    unsigned int length;
}ZlList;

ZlRetVal ZlInitList(ZlList *L);     /*初始化操作，建立一个空的线性表*/ 
ZlRetVal ZlListEmpty(ZlList L);     /*线性表为空，返回0，否则返回-1*/
ZlRetVal ZlClearList(ZlList *L);    /*将线性表清空*/
ZlRetVal ZlGetElem(ZlList L, unsigned int Index, ElemType *Ret);        /*将线性表L中的第Index个元素返回给Ret*/
unsigned int ZlLocateElem(ZlList L, ElemType Obj);                      /*在线性表L中查找Obj元素，查找成功返回该元素在表中的索引，否则，返回0*/
ZlRetVal ZlListInsert(ZlList *L, unsigned int Index, ElemType Obj);     /*在线性表L中的第Index个位置插入新元素Obj*/
ZlRetVal ZlListDelete(ZlList *L, unsigned int Index, ElemType *Ret);    /*删除线性表L中的第Index个元素，并用Ret返回其值*/
unsigned int ZlListLength(ZlList *L);   /*返回线性表L的元素个数*/

#ifdef __cplusplus
}
#endif

#endif