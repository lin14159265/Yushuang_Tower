

#ifndef __MLX90614_H
#define __MLX90614_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define	NACK 1  //不应答或否定的应答
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //表面温度值
float SMBus_ReadTemphr(void);//环境温度值
void SDA_OUT(void);
void SDA_IN(void);


void IIC_Ack(unsigned char re);	  //发送应答信号
unsigned char IIC_WaitAck(void);        //等待应答
void IIC_WriteByte(unsigned char Temp);
unsigned char IIC_Read_Byte(void);
#endif

/*********************************END OF FILE*********************************/
