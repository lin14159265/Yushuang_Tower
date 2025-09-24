#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define USART2_RX_ENABLE     1      // 是否开启接收功能
#define USART2_TXBUFF_SIZE   1024   // 发送缓冲区大小

#if USART2_RX_ENABLE
#define USART2_RXBUFF_SIZE   1024   // 接收缓冲区大小
extern char Usart2_RxCompleted;
extern unsigned int Usart2_RxCounter;
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE];
#endif

// 函数声明
void Usart2_Init(unsigned int bound);
void u2_printf(char* fmt, ...);
void u2_TxData(unsigned char *data);

#endif