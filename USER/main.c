#include "stm32f10x.h"      #include "string.h"
//#include "LCD_calculate.h"
//#include "Lcd_Driver.h"
#include "usart1.h"
#include "stdio.h" 
#include "delay.h"
#include "usart2.h"
#include "lcd.h"
#include "pic.h"

// 1. 先在程序顶部定义好你的连接信息
#define DEVICE_NAME  "test"
#define PRODUCT_ID   "nZ4v9G1iDK"
// 注意：这里的密码字符串，里面的 % 符号需要用 %% 来转义
#define AUTH_INFO    "version=2018-10-31&res=products%%2FnZ4v9G1iDK%%2Fdevices%%2Ftest&et=1798497693&method=md5&sign=ZmzDSu0enWpLqIS8rHDjXw%%3D%%3D"
int main(void)
{
    char cmd_buffer[512];
    float Temp1, Temp2, Temp3, Temp4, diffTemp, windSpeed, humidity;
    int status=0;
    char json_buffer[256];
    char pub_topic[128];
    char pub_cmd[512];

    long message_id = 100; // 定义一个消息ID，让它每次都变化 
    sprintf(pub_topic, "$sys/%s/%s/dp/post/json", PRODUCT_ID, DEVICE_NAME);



    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Usart1_Init(115200); 

    delay_ms(500);
    

    
    // --- 2. 初始化BC260Y并连接OneNET (单向指令流) ---
    u1_printf("AT\r\n");
    delay_ms(5000);

    u1_printf("ATE0\r\n"); // 关闭回显，让通信更干净
    delay_ms(5000);

    u1_printf("AT+CSQ\r\n"); // 附着网络
    delay_ms(2000);
    

    u1_printf("AT+CGATT=1\r\n"); // 附着网络
    delay_ms(2000);

    u1_printf("AT+QMTCFG=\"version\",0,4\r\n"); // 设置MQTT版本为3.1.1
    delay_ms(5000);

    u1_printf("AT+QMTOPEN=0,\"mqtts.heclouds.com\",1883\r\n"); // 打开网络通道
    delay_ms(3000);

    // 3. 使用 sprintf 将所有部分格式化到缓冲区中
    sprintf(cmd_buffer, "AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n", DEVICE_NAME, PRODUCT_ID, AUTH_INFO);

    // 4. 发送最终构造好的指令字符串
    u1_printf(cmd_buffer);


    while(1)
    {
        

     // 1. 模拟一个温度数据
        float temperature = 25.8;
        message_id++;

        // 2. 构造JSON数据
        sprintf(json_buffer, "{\"id\":\"%ld\",\"dp\":{\"temp\":[{\"v\":%.1f}]}}",
                message_id, temperature);

        // --- 3. 严格按照教程的两步法发送数据 ---
        
        // --- 第一步：发送指令头 ---
        // 构造指令头，包含Topic和JSON数据的长度
        sprintf(pub_cmd, "AT+QMTPUB=0,0,0,0,\"%s\",%d\r\n", pub_topic, strlen(json_buffer));
        
        // 发送指令头
        u1_printf(pub_cmd);
        
        // 关键延时：给模块足够的时间处理指令头，并返回 '>' 字符
        // 虽然我们不接收它，但必须等待这个过程完成
        delay_ms(500); 

        delay_ms(10000); 
        
        // --- 第二步：发送JSON数据体 ---
        u1_printf(json_buffer);
        
        // 发送完数据后，也需要延时，等待模块将数据通过网络发出去
        delay_ms(2000); 
        
        // --- 本轮上报结束，进入下一轮的延时 ---
        delay_ms(10000); // 每10秒（外加之前的延时）上报一次
        
        
    


        



    }   
}


