/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*          实现各种中断服务函数的头文件           */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

/*-------------------------------------------------*/
/*                                                 */
/*                    系统中断                     */
/*                                                 */
/*-------------------------------------------------*/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);


/*-------------------------------------------------*/
/*                                                 */
/*                    应用中断                     */
/*                                                 */
/*-------------------------------------------------*/


#endif 
