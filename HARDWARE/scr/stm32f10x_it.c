

#include "stm32f10x.h"     //包含需要的头文件
#include "stm32f10x_it.h"  //包含需要的头文件
#include "main.h"          //包含需要的头文件
#include "delay.h"         //包含需要的头文件
#include "usart1.h"        //包含需要的头文件
#include "timer1.h"        //包含需要的头文件
#include "led.h"           //包含需要的头文件
#include "global.h"


/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
		
		timecount1++;
		timecount2++;
		timecount3++;
		timecount4++;
		timecount5++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：不可屏蔽中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：硬件出错后进入的中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：内存管理中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：预取指失败，存储器访问失败中断处理函数   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：未定义的指令或非法状态处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：软中断，SWI 指令调用的处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：调试监控器处理函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：可挂起的系统服务处理函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：SysTic系统嘀嗒定时器处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}


void USART1_IRQHandler(void)
{
    // 检查是否是接收中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // 读取接收到的数据
        uint8_t received_data = USART_ReceiveData(USART1);
        
        // 简化：只做必要的缓冲区操作
        if (usart1_rx_len < USART1_RXBUFF_SIZE - 1)
        {
            usart1_rx_buffer[usart1_rx_len++] = received_data;
            usart1_rx_buffer[usart1_rx_len] = '\0'; // 及时添加结束符
        }
        else
        {
            // 缓冲区满了，从头开始
            usart1_rx_len = 0;
            usart1_rx_buffer[usart1_rx_len++] = received_data;
            usart1_rx_buffer[usart1_rx_len] = '\0';
        }
        
        // 重要：手动清除中断标志
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    // 清除溢出错误
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
}




// 在stm32f10x_it.c中添加
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART2);
        
#if USART2_RX_ENABLE
        if(Usart2_RxCounter < USART2_RXBUFF_SIZE - 1)
        {
            Usart2_RxBuff[Usart2_RxCounter++] = data;
            Usart2_RxBuff[Usart2_RxCounter] = '\0';
        }
#endif
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
