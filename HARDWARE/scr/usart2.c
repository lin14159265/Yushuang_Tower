
#include "stm32f10x.h"  //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "global.h"

#if  USART2_RX_ENABLE                   //如果使能接收功能
char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

/*-------------------------------------------------*/
/*函数名：初始化串口2发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if USART2_RX_ENABLE
    NVIC_InitTypeDef NVIC_InitStructure;
#endif

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART2);
    
    // GPIO配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    // USART配置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
#if USART2_RX_ENABLE
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
#else
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
#endif
        
    USART_Init(USART2, &USART_InitStructure);

    // 重要：先使能USART，再配置中断
    USART_Cmd(USART2, ENABLE);

#if USART2_RX_ENABLE  	         						
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}

/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__attribute__((aligned(8)))char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
    unsigned int i, length;
    
    va_list ap;
    va_start(ap, fmt);
    vsprintf(USART2_TxBuff, fmt, ap);
    va_end(ap);	
    
    length = strlen((const char*)USART2_TxBuff);		
    
    for(i = 0; i < length; i++)
    {			
        // 修正：等待发送缓冲区为空（TXE标志置位）
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        
        USART_SendData(USART2, USART2_TxBuff[i]);
        
        // 可选：等待发送完成（TC标志置位）- 对于连续发送可能不需要
        // while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    
    // 等待最后一个字节发送完成
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	int	i;	
	while((USART2->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		USART2->DR = data[i+1];
		while((USART2->SR&0X40)==0);	
	}
}
