/*
 * @Author: your name
 * @Date: 2020-05-18 18:20:33
 * @LastEditTime: 2020-05-23 00:23:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /idh/vendor/mbtk/FCWL_APP/base.c
 */
#include "base.h"

#define HIGH_BYTE 0X00
#define LOW_BYTE 0X01

void BigEndianToArr(unsigned char *buff, unsigned short sval)
{
    if (NULL == buff) return;

    buff[HIGH_BYTE] = (unsigned char)(sval >> 8);
    buff[LOW_BYTE] = (unsigned char)sval;
}

unsigned short ArrtoBigEndian(const unsigned char *buff)
{
    if (NULL == buff) return 0;

    return (buff[HIGH_BYTE]*0xff+buff[LOW_BYTE]);
}

unsigned short ShortHighLowSwap(unsigned short sval)
{
    return ((sval & 0x00ff) << 8) + ((sval & 0xff00) >> 8);
}

unsigned char CheckXor(const unsigned char *buff, unsigned char buffLen)
{
    if (NULL == buff || buffLen == 0) return -1;

    unsigned char checkXor = 0;
    int i = 0;
    for (i; i < buffLen; i++)
    {
        checkXor ^= (unsigned char)buff[i];
    }

    return checkXor;
}

static void LinuxTmToLocalTm(const struct tm *linuxTm, struct LocalTime *localTm)
{
    if (NULL == linuxTm || NULL == localTm) return;
    
    localTm->year = (unsigned char)(linuxTm->tm_year - 100);     //! 本地时间从2000起，Linux从1900起
    localTm->mon = (unsigned char)(linuxTm->tm_mon + 1);         //! linux tm_mon: 0-11
    localTm->day = (unsigned char)linuxTm->tm_mday;
    localTm->hour = (unsigned char)linuxTm->tm_hour;             //! 0~23 
    localTm->min = (unsigned char)linuxTm->tm_min;
    localTm->sec = (unsigned char)linuxTm->tm_sec;
}

static time_t LocalTimeToUTC(const struct LocalTime *localTm)
{
    if (NULL == localTm) return -1;

    unsigned int year = (unsigned int)localTm->year + 2000;
    unsigned int mon = (unsigned int)localTm->mon;
    unsigned int day = (unsigned int)localTm->day;
    unsigned int hour = (unsigned int)localTm->hour;
    unsigned int min = (unsigned int)localTm->min;
    unsigned int sec = (unsigned int)localTm->sec;
    
    if (0 >= (int)(mon -= 2))
    {              /* 1..12 -> 11,12,1..10 */
        mon += 12; /* Puts Feb last since it has leap day */
        year -= 1;
    }
    
    return ((((unsigned long)(year / 4 - year / 100 + year / 400 + 367 * mon / 12 + day) + year * 365 - 719499) 
    * 24 + hour /* now have hours */) 
    * 60 +min /* now have minutes */) 
    * 60 + sec; /* finally seconds */
}

int SetLocalTime(struct LocalTime localTm)
{
    struct timeval tv; 
    time_t localSec = LocalTimeToUTC(&localTm);
    //// localSec -= 28800; //! 减去时区8小时才为国际UTC时间
    tv.tv_sec = localSec;  
    tv.tv_usec = 0;  
    DBG_PRINTF("tv.tv_sec %ld \n", tv.tv_sec); 
    if(settimeofday (&tv, (struct timezone *) 0) < 0)  
    {  
        DBG_PRINTF("Set system datatime error! \n");  
        return -1;  
    }
    
    return 0;
}

void GetLocalTime(struct LocalTime *localTm)
{
    if (NULL == localTm) return;

    struct tm *ptm;
    time_t timep;

    time(&timep);
    ptm = localtime(&timep);
    LinuxTmToLocalTm(ptm, localTm);
}