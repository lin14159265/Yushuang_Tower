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

    
    



    /*
    Lcd_Init();
    Lcd_Clear(GRAY0); 
    char buffer[20];
    
    // -- 绘制标题区 --
    Gui_DrawFont_GBK16(5, 2, BLUE, GRAY0, "御霜塔-霜冻预警"); 
    //Gui_DrawFillRect(0, 0, 139, 20, BLUE);  // 标题蓝底
    //Gui_DrawFont_GBK16(5, 2, WHITE, BLUE, "御霜塔-霜冻预警");  // 白字蓝底
    Gui_DrawLine(0, 20, 139, 20, GRAY1);  // 分隔线
    
    // -- 绘制核心数据区 (温度相关) --
    
    Gui_DrawFont_GBK16(5-4, 22, BLACK, GRAY0, "T1:");
    Gui_DrawFont_GBK16(69-4, 22, BLACK, GRAY0, "T2:");   
    Gui_DrawFont_GBK16(5-4, 42, BLACK, GRAY0, "T3:");  
    Gui_DrawFont_GBK16(69-4, 42, BLACK, GRAY0, "T4:");

    Temp1=88.88;
    Temp2=88.88;
    Temp3=88.88;
    Temp4=88.88;

    sprintf(buffer, "%.2f", Temp1);
    Gui_DrawFont_GBK16(28-4, 22, BLUE, GRAY0, buffer);

    sprintf(buffer, "%.2f", Temp2);
    Gui_DrawFont_GBK16(92-4, 22, BLUE, GRAY0, buffer);

    sprintf(buffer, "%.2f", Temp3);
    Gui_DrawFont_GBK16(28-4, 42, BLUE, GRAY0, buffer);

    sprintf(buffer, "%.2f", Temp4);
    Gui_DrawFont_GBK16(92-4, 42, BLUE, GRAY0, buffer);
    
    Gui_DrawFont_GBK16(0, 62, BLACK, GRAY0, " Amb Temp:");
    sprintf(buffer, "%.2f", diffTemp);
    Gui_DrawFont_GBK16(80, 62, GREEN, GRAY0, buffer);
    
    Gui_DrawLine(0, 80, 139, 80, GRAY1);  // 分隔线
    
    // -- 绘制辅助数据区 --
    // 风速
    Gui_DrawFont_GBK16(5, 82, BLACK, GRAY0, "风速:");
    sprintf(buffer, "%.1f m/s", windSpeed);
    Gui_DrawFont_GBK16(60, 82, BLACK, GRAY0, buffer);  // 对齐标签Y坐标  
    // 湿度
    Gui_DrawFont_GBK16(5, 103, BLACK, GRAY0, "湿度:");
    sprintf(buffer, "%d %%", (int)humidity);
    Gui_DrawFont_GBK16(60, 103, BLACK, GRAY0, buffer);   
    Gui_DrawLine(0, 122, 139, 122, GRAY1);  // 分隔线   
    // -- 绘制系统状态区 --
    Gui_DrawFont_GBK16(5, 124, BLACK, GRAY0, "状态:");
    // 底部状态条
    //Gui_DrawFillRect(0, 146, 139, 160, GRAY2);
    Gui_DrawFont_GBK16(5, 142, WHITE, GRAY2, "系统正常运行中");

    DisplayButtonDown(118,150,128,160);

    DisplayButtonUp(118-10,150-10,128-10,160-10);

    */



    LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    //LCD_ShowPicture(0,0,80,100,gImage_cat2);

    

    
    while(1)
    {
        /*
        
        // 根据状态显示不同颜色和文字
    switch(status) {
        case 0:  // 安全
            //Gui_DrawFillRect(55, 125, 135, 145, GREEN);  // 绿色背景块
            Gui_DrawFont_GBK16(60, 124, GREEN, GRAY0, "安 全");
            break;
        case 1:  // 注意
            //Gui_DrawFillRect(55, 125, 135, 145, YELLOW);
            Gui_DrawFont_GBK16(60, 124, YELLOW, GRAY0, "注 意");
            break;
        case 2:  // 预警
            //Gui_DrawFillRect(55, 125, 135, 145, ORANGE);
            Gui_DrawFont_GBK16(60, 124, YELLOW, GRAY0, "预 警");
            break;
        case 3:  // 紧急
            //Gui_DrawFillRect(55, 125, 135, 145, RED);
            Gui_DrawFont_GBK16(60, 124, RED, GRAY0, "紧 急");
            break;
    }
    status++;
    if(status==4)
    {
        status=0;
    }
    */

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


