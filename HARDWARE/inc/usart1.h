#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h> // 引入标准整数类型

/*---------- 配置宏 ----------*/
#define USART1_RX_ENABLE 1          // 确定开启串口1接收
#define USART1_RXBUFF_SIZE 256      // 统一定义缓冲区大小
#define USART1_TXBUFF_SIZE 256      // 统一定义发送缓冲区大小

/*---------- 全局变量声明 (公共仓库蓝图) ----------*/
// 使用 extern 关键字，告诉其他文件这些变量是存在的
extern uint8_t usart1_rx_buffer[USART1_RXBUFF_SIZE];
extern volatile uint16_t usart1_rx_len; // 声明为 volatile，防止编译器优化

/*---------- 函数声明 ----------*/
void Usart1_Init(unsigned int bound);
void u1_printf(char* fmt,...);

// 在 usart1.h 的函数声明区域添加下面两行
void clear_usart1_buffer(void);
char* get_usart1_buffer(void);

#endif