#include "bluetooth.h"
#include "delay.h"  // 工程中已有的延时函数

// 全局变量：接收缓冲区
unsigned char bluetooth_rx_buf[BLUETOOTH_RX_BUF_SIZE] = {0};
unsigned int bluetooth_rx_len = 0;

/**
 * 初始化USART3用于蓝牙通信
 * @param baudrate：波特率（如38400，HC-05默认波特率）
 */
void Bluetooth_Init(unsigned int baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能时钟：USART3和GPIOB（USART3挂载在APB1，GPIOB在APB2）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 配置USART3_TX（PB10：复用推挽输出）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置USART3_RX（PB11：浮空输入）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置USART3参数
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       // 1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;          // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式
    USART_Init(USART3, &USART_InitStructure);

    // 配置接收中断（参考usart1.c的中断配置）
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  // 使能接收中断

    // 配置中断优先级（工程中统一用NVIC_PriorityGroup_2）
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占优先级1（低于USART1的0）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         // 子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能USART3
    USART_Cmd(USART3, ENABLE);
}

/**
 * 发送单个字节
 */
void Bluetooth_SendByte(unsigned char data) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  // 等待发送缓冲区空
    USART_SendData(USART3, data);
}

/**
 * 发送指定长度的数据
 */
void Bluetooth_SendData(unsigned char *data, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        Bluetooth_SendByte(data[i]);
    }
}

/**
 * 发送字符串（需包含'\0'结尾）
 */
void Bluetooth_SendString(char *str) {
    Bluetooth_SendData((unsigned char *)str, strlen(str));
}

/**
 * 发送AT指令并等待响应
 * @param cmd：AT指令（如"AT+NAME"）
 * @param reply：预期响应（如"OK"）
 * @param timeout：超时时间（单位：10ms，如100=1秒）
 * @return 1：成功收到响应；0：超时失败
 */
char Bluetooth_SendATCmd(char *cmd, char *reply, unsigned int timeout) {
    bluetooth_rx_len = 0;  // 清空接收缓冲区
    memset(bluetooth_rx_buf, 0, BLUETOOTH_RX_BUF_SIZE);

    // 发送AT指令（末尾需加回车换行"\r\n"）
    Bluetooth_SendString(cmd);
    Bluetooth_SendString("\r\n");

    // 等待响应
    while (timeout--) {
        delay_ms(10);  // 工程中已有的毫秒延时函数
        if (bluetooth_rx_len > 0 && strstr((char *)bluetooth_rx_buf, reply) != NULL) {
            return 1;  // 收到预期响应
        }
    }
    return 0;  // 超时
}

/**
 * USART3中断服务函数（处理蓝牙接收）
 */
void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {  // 接收中断触发
        if (bluetooth_rx_len < BLUETOOTH_RX_BUF_SIZE - 1) {  // 避免缓冲区溢出
            bluetooth_rx_buf[bluetooth_rx_len++] = USART_ReceiveData(USART3);  // 读取数据
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);  // 清除中断标志
    }
}