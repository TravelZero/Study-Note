#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h> //标准输入输出定义
#include <stdlib.h> //标准函数库定义
#include <unistd.h> //Unix标准函数定义
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
#include <termios.h> //POSIX中断控制定义
#include <errno.h> //错误号定义

int     UartInitDev(char *pDev, int nSpeed, int databits, int stopbits, int parity);
int     UartCloseDev(int fd);
int     UartSetSpeed(int fd, int nSpeed);
int     UartSetParity(int fd, int databits, int stopbits, int parity);
ssize_t UartSendData(int fd, char *pData, size_t len);
ssize_t UartRecData(int fd, char *pData, size_t len);

#ifdef __cplusplus
}
#endif

#endif