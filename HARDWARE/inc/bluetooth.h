#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include <string.h>

// 蓝牙接收缓冲区配置
#define BLUETOOTH_RX_BUF_SIZE 128  // 缓冲区大小
extern unsigned char bluetooth_rx_buf[BLUETOOTH_RX_BUF_SIZE];  // 接收缓冲区
extern unsigned int bluetooth_rx_len;  // 接收数据长度

// 函数声明
void Bluetooth_Init(unsigned int baudrate);  // 初始化蓝牙串口（USART3）
void Bluetooth_SendByte(unsigned char data);  // 发送单个字节
void Bluetooth_SendData(unsigned char *data, unsigned int len);  // 发送数据
void Bluetooth_SendString(char *str);  // 发送字符串
char Bluetooth_SendATCmd(char *cmd, char *reply, unsigned int timeout);  // 发送AT指令并等待响应

#endif