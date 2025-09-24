#include "stm32f10x.h"
#include "string.h"
#include "usart1.h"
#include "stdio.h" 
#include "delay.h"
#include "usart2.h" // 如果你没有用调试串口，可以注释掉
#include "lcd.h"
#include "pic.h"
#include "led.h"

// --- 1. OneNET 连接信息 ---
#define DEVICE_NAME  "test"
#define PRODUCT_ID   "nZ4v9G1iDK"
#define AUTH_INFO    "version=2018-10-31&res=products%%2FnZ4v9G1iDK%%2Fdevices%%2Ftest&et=1798497693&method=md5&sign=ZmzDSu0enWpLqIS8rHDjXw%%D%%D"

// --- OneNET 主题信息 ---
#define PUB_TOPIC    "$sys/"PRODUCT_ID"/"DEVICE_NAME"/dp/post/json"
// OneNET 标准命令下发主题, `+` 是通配符
#define SUB_TOPIC    "$sys/"PRODUCT_ID"/"DEVICE_NAME"/cmd/request/+" 

// 函数声明
int send_cmd(const char* cmd, const char* expected_rsp, uint32_t timeout_ms);
void parse_command(const char* buffer);

int main(void)
{
    char cmd_buffer[512];
    char json_buffer[256];
    long message_id = 100;

    // 初始化
    Usart1_Init(115200);
    Usart2_Init(115200);
    LED_Init();
    
    // 测试中断是否工作
    u2_printf("Starting USART1 test...\r\n");
    
    // 发送测试命令
    u1_printf("AT\r\n");
    
    // 等待并检查中断
    delay_ms(100);
    u2_printf("Interrupt test - Buffer length: %d, Content: %s\r\n", 
           usart1_rx_len, get_usart1_buffer());
    
    if(usart1_rx_len > 0) {
        u2_printf("Interrupt is working!\r\n");
    } else {
        u2_printf("Interrupt NOT working - checking configuration...\r\n");
    }
    
    
    
    // 检查模块通信
    while(send_cmd("AT\r\n", "OK", 1000));

    send_cmd("ATE0\r\n", "OK", 1000);

    // 等待网络附着成功
    while(send_cmd("AT+CGATT?\r\n", "+CGATT: 1", 2000));
    
    send_cmd("AT+QMTCFG=\"version\",0,4\r\n", "OK", 3000);

    // !! 关键修正: OneNET非加密MQTT地址是 mqtt.heclouds.com !!
    if(send_cmd("AT+QMTOPEN=0,\"mqtt.heclouds.com\",1883\r\n", "+QMTOPEN: 0,0", 8000))
    {
        // 如果打开网络失败，可以在这里处理，比如重启
        // for(;;);
    }
    
    sprintf(cmd_buffer, "AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n", DEVICE_NAME, PRODUCT_ID, AUTH_INFO);
    if(send_cmd(cmd_buffer, "+QMTCONN: 0,0,0", 8000))
    {
        // 连接失败
        // for(;;);
    }

    // --- 3. 订阅命令下发主题 ---
    sprintf(cmd_buffer, "AT+QMTSUB=0,1,\"%s\",1\r\n", SUB_TOPIC);
    send_cmd(cmd_buffer, "+QMTSUB: 0,1,0", 5000); // QoS 1, 成功会返回 +QMTSUB: 0,1,0,1, 我们只匹配前面

    while(1)
    {
        // --- 4. 检查是否有服务器下发的命令 ---
        if(strstr(get_usart1_buffer(), "+QMTRECV:"))
        {
            parse_command(get_usart1_buffer());
            clear_usart1_buffer();
        }

        // --- 5. 构造并上报数据 ---
        float temperature = 25.8; // 模拟温度
        message_id++;

        sprintf(json_buffer, "{\"id\":\"%ld\",\"dp\":{\"temp\":[{\"v\":%.1f}]}}",
                message_id, temperature);

        // --- 使用AT指令发布数据 ---
        sprintf(cmd_buffer, "AT+QMTSUB=0,0,0,0,\"%s\",%d\r\n", PUB_TOPIC, strlen(json_buffer));
        
        // 第一步：发送指令头，等待 '>' 符号
        if(send_cmd(cmd_buffer, ">", 2000) == 0)
        {
            // 第二步：收到'>'后，立即发送JSON数据，并等待最终的成功响应
            send_cmd(json_buffer, "+QMTPUB: 0,0,0", 5000);
        }
        
        delay_ms(15000); // 每15秒上报一次
    }   
}


/**
 * @brief 解析服务器下发的命令
 * @param buffer 包含+QMTRECV的串口接收缓冲区内容
 */

 
void parse_command(const char* buffer)
{
    // 简单的解析示例，实际应用中可能需要更健壮的解析
    // URC格式: +QMTRECV: 0,<msgid>,"<topic>","<payload>"
    char* payload_start = strstr(buffer, ",\""); // 找到第一个 ,"
    if (!payload_start) return;
    payload_start = strstr(payload_start + 2, ",\""); // 找到第二个 ,"
    if (!payload_start) return;
    payload_start = strstr(payload_start + 2, ",\""); // 找到第三个 ,"
    if (!payload_start) return;
    payload_start += 2; // 指向负载的起始 "

    char* payload_end = strrchr(buffer, '\"'); // 从后往前找最后一个 "

    if (payload_start && payload_end && payload_end > payload_start)
    {
        char command[128] = {0};
        int len = payload_end - payload_start -1;
        if(len > 0 && len < 128)
        {
             strncpy(command, payload_start+1, len);
             // 打印收到的命令（需要有调试串口）
             // printf("Received command: %s\r\n", command);

             // --- 在这里添加你的控制逻辑 ---
             if (strcmp(command, "LED_ON") == 0)
             {
                 // 控制LED灯亮
             }
             else if (strcmp(command, "LED_OFF") == 0)
             {
                 // 控制LED灯灭
             }
        }
    }
}


// 在你的led.h或者main.c顶部，确保有类似这样的宏定义
// 假设LED1连接到某个GPIO，低电平点亮
// #define LED1_ON()  GPIO_ResetBits(GPIOC, GPIO_Pin_13)
// #define LED1_OFF() GPIO_SetBits(GPIOC, GPIO_Pin_13)


/**
 * @brief 发送AT指令并等待预期响应（带LED调试）
 * @param cmd 要发送的指令
 * @param expected_rsp 期待的响应字符串，例如 "OK"
 * @param timeout_ms 超时时间
 * @return 0: 成功, 1: 失败/超时
 */
int send_cmd(const char* cmd, const char* expected_rsp, uint32_t timeout_ms)
{   
    delay_ms(50); 
    clear_usart1_buffer();
    u1_printf((char*)cmd);
    
    uint32_t time_elapsed = 0;
    uint32_t check_interval = 10; // 减小检查间隔
    
    while(time_elapsed < timeout_ms)
    {       
        delay_ms(check_interval);
        time_elapsed += check_interval;
        
        if (usart1_rx_len > 0)
        {
            // 确保字符串正确终止
            usart1_rx_buffer[usart1_rx_len] = '\0';
            
            // 调试输出：可以添加串口打印接收到的数据
            // printf("Received: %s\r\n", get_usart1_buffer());
            
            if (strstr(get_usart1_buffer(), expected_rsp) != NULL)
            {
                return 0; // 成功
            }
            
            // 检查是否有错误响应
            if (strstr(get_usart1_buffer(), "ERROR") != NULL)
            {
                return 1; // 错误
            }
        }
    }
    return 1; // 超时
}




