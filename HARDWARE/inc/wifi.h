

#ifndef __WIFI_H
#define __WIFI_H

#include "usart2.h"	    //包含需要的头文件

#define RESET_IO(x)    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)x)  //PF1控制WiFi的复位

#define WiFi_printf       u2_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart2_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Usart2_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //串口2控制 WiFi

//#define SSID   "OPPO"                     //路由器SSID名称
//#define PASS   "z1234567"                 //路由器密码


#define SSID   "UETai"                     //路由器SSID名称
#define PASS   "12345678900"                 //路由器密码


void WiFi_ResetIO(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);


#endif


