#include "usart1.h"

/*---------- 全局变量定义 (建造仓库实体) ----------*/
uint8_t usart1_rx_buffer[USART1_RXBUFF_SIZE];
volatile uint16_t usart1_rx_len = 0; // 定义为 volatile

// 发送缓冲区
__attribute__((aligned(8))) char Usart1_TxBuff[USART1_TXBUFF_SIZE];

/*-------------------------------------------------*/
/*函数名：初始化串口1                              */
/*-------------------------------------------------*/
void Usart1_Init(unsigned int bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // PA9 - TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10 - RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1 配置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 开启接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC 配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级可以根据你的系统调整
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

/*-------------------------------------------------*/
/*函数名：串口1 printf函数                         */
/*-------------------------------------------------*/
void u1_printf(char* fmt,...) 
{
    unsigned int i, length;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(Usart1_TxBuff, fmt, ap);
    va_end(ap);
    
    length = strlen((const char*)Usart1_TxBuff);
    for(i = 0; i < length; i++)
    {
        USART_SendData(USART1, Usart1_TxBuff[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

// 在 usart1.c 的文件末尾添加这两个函数

/**
 * @brief 清空串口1的接收缓冲区
 */
void clear_usart1_buffer(void)
{
    memset(usart1_rx_buffer, 0, USART1_RXBUFF_SIZE);
    usart1_rx_len = 0;
}

/**
 * @brief 获取串口1接收缓冲区的指针
 * @return 缓冲区的只读指针
 */
const char* get_usart1_buffer(void)
{
    return (const char*)usart1_rx_buffer;
}

