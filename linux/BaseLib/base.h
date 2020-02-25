/*
 * @Author: your name
 * @Date: 2020-05-18 18:17:12
 * @LastEditTime: 2020-05-20 01:00:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /idh/vendor/mbtk/FCWL_APP/base.h
 */ 
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h> //Unix标准函数定义
#include <time.h>

#include "debug.h"

struct LocalTime
{
    unsigned char year;
    unsigned char mon;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
};

/* 将 sval 按大端模式写入 buff 数组内 */
void BigEndianToArr(unsigned char *buff, unsigned short sval);

/* 将 buff 数组内数据按大端模式返回 */
unsigned short ArrtoBigEndian(const unsigned char *buff);

unsigned short ShortHighLowSwap(unsigned short sval);

unsigned char CheckXor(const unsigned char *buff, unsigned char buffLen);

int SetLocalTime(struct LocalTime localTm);

void GetLocalTime(struct LocalTime *localTm);

static void LinuxTmToLocalTm(const struct tm *linuxTm, struct LocalTime *localTm);

static time_t LocalTimeToUTC(const struct LocalTime *localTm);

#ifdef __cplusplus
}
#endif

#endif