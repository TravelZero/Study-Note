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

#include "debug.h"

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1

int GpioExport(int pin);
int GpioUnexport(int pin);
int GpioDirection(int pin, int dir);
int GpioRead(int pin);
int GpioWrite(int pin, int value);

#ifdef __cplusplus
}
#endif

#endif