/*
 * @Author: your name
 * @Date: 2020-05-19 19:34:41
 * @LastEditTime: 2020-05-19 20:36:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /idh/vendor/mbtk/FCWL_APP/debug.h
 */ 
#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h> //标准输入输出定义
#include <stdlib.h> //标准函数库定义
#include <unistd.h> //Unix标准函数定义

#define GUARD(ret) \
  ({                  \
    if (ret == -1) {  \
      return -1;      \
    }                 \
  })

#define DEBUG 1

#if DEBUG
#define DBG_PRINTF(fmt, args...) \
do\
{\
    printf("<<File:%s Line:%d Function:%s>>", __FILE__, __LINE__, __FUNCTION__);\
    printf(fmt, ##args);\
}while(0)

#else

#define DBG_PRINTF(fmt, args...)

#endif

#ifdef __cplusplus
}
#endif

#endif