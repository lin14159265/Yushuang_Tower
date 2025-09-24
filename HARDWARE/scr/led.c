

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);   	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);                             
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                                             //准备设置PC13状态指示灯
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		                              //推免输出方式
	GPIO_Init(GPIOC, &GPIO_InitStructure);            		                              
	

	
	
	   //复位
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                 //准备设置PB3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //推免输出方式
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		  //设置PB3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                 //准备设置PB3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //推免输出方式
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		  //设置PB3
	
//	LED_AllOff();   //所有LED熄灭

 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 



}




/*-------------------------------------------------*/
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
 	 LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
  LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}



