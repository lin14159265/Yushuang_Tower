#include "stm32f10x.h"
#include "string.h"
#include "usart1.h"
#include "stdio.h" 
#include "delay.h"
// #include "usart2.h" // 如果你没有用调试串口，可以注释掉
#include "lcd.h"
#include "pic.h"

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

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Usart1_Init(115200);
    // Usart2_Init(115200); // 建议开启一个调试串口，方便看log
    Delay_Init(); // 你的delay.c里有这个函数，最好调用一下

    // --- 2. 初始化BC260Y并连接OneNET (可靠的交互式流程) ---
    
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

/**
 * @brief 发送AT指令并等待预期响应（简化版）
 * @param cmd 要发送的指令
 * @param expected_rsp 期待的响应字符串，例如 "OK"
 * @param timeout_ms 超时时间
 * @return 0: 成功, 1: 失败/超时
 */
int send_cmd(const char* cmd, const char* expected_rsp, uint32_t timeout_ms)
{
    clear_usart1_buffer();
    u1_printf((char*)cmd); // 发送指令
    
    // 如果你有调试串口，在这里打印发送的指令会很有帮助
    // u2_printf("SEND: %s", cmd);

    uint32_t time_elapsed = 0;
    while(time_elapsed < timeout_ms)
    {
        delay_ms(10);
        time_elapsed += 10;
        
        // 检查接收缓冲区中是否包含期望的响应
        if (strstr(get_usart1_buffer(), expected_rsp) != NULL)
        {
            // u2_printf("RECV: %s\r\n", get_usart1_buffer()); // 调试输出
            return 0; // 找到了，成功
        }
    }
    
    // u2_printf("TIMEOUT for %s\r\n", cmd); // 调试输出
    // u2_printf("Buffer content: %s\r\n", get_usart1_buffer());
    return 1; // 超时
}

