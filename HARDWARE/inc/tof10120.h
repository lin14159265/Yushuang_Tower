#ifndef __TOF10120_H
#define __TOF10120_H

#include "stm32f10x.h"

#define I2C_DEVID    		100

unsigned char SensorWritenByte(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size);
unsigned char SensorReadnByte(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);
unsigned int Read_Distance(void);
unsigned char tof10120(void);


#endif



