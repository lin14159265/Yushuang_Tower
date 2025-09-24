#include "beep.h"
	    
//蜂鸣器初始化
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOA端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //BEEP-->PA0 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 //GPIO_ResetBits(GPIOB,GPIO_Pin_8);//输出0，关闭蜂鸣器输出
}


