#include "stm32f10x.h"
#include "mlx90614.h"
#include "tof10120.h"
#include "delay.h"

unsigned char devid;


//TOF10120write
unsigned char SensorWritenByte(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size)
{
	unsigned char i = 0;
  
	SMBus_StartBit();
  IIC_WriteByte(Devid|0x00);
  if( IIC_WaitAck()==0xff )
  {
		SMBus_StartBit();
    return 0xff;
  }
  IIC_WriteByte( SubAdd&0xff );
  if(  IIC_WaitAck()==0xff )
  {
			SMBus_StopBit();
			return 0xff;
  }
	delay_us(10);
	for ( i = 0; i < Size; i++)
	{
		IIC_WriteByte(TXBuff[Size-i-1]);
		 if(  IIC_WaitAck()==0xff )
		{
			SMBus_StopBit();
			return 0xff;
		}
	}
 	SMBus_StopBit();
  return 0;
}


//TOF10120read
unsigned char SensorReadnByte(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size)
{	
	unsigned char i = 0;
	
	SMBus_StartBit();
  IIC_WriteByte(Devid|0x00 );
 if(  IIC_WaitAck()==0xff )
  {
			SMBus_StopBit();
			return 0xff;
  }
  IIC_WriteByte( SubAdd&0xff );
 if(  IIC_WaitAck()==0xff )
  {
			SMBus_StopBit();
			return 0xff;
  }	
		SMBus_StopBit();
		delay_us(50);
		SMBus_StartBit();
		IIC_WriteByte( Devid|0x01);
 if(IIC_WaitAck()==0xff )
  {
			SMBus_StopBit();
			return 0xff;
  }
	for ( i = 0; i < Size; i++)
	{
		RXBuff[Size-i-1] = IIC_Read_Byte();	
		if((i+1)==Size)			
			IIC_Ack(1);
		else
			IIC_Ack(0);
	}
 	SMBus_StopBit();
	
  return 1;
}

//return 1 is ok.
unsigned char tof10120(void)
{
  unsigned char tempv=0;
	unsigned int maxv;
	
	devid=I2C_DEVID;
	tempv=1;			//=0串口主动发送 =1串口或者I2C被动读取
	SensorWritenByte(devid,(unsigned char *)&tempv, 0x09, 1);	
	tempv=0;//=0滤波值 =1实时值
	SensorWritenByte(devid,(unsigned char *)&tempv, 0x08, 1); 
	delay_ms(100);
  SensorReadnByte(devid,(unsigned char *)&maxv, 0x0c,2);
	if((maxv&0xffff)==0xffff)return 0;
	return 1;
}

 //读取距离
unsigned int Read_Distance(void)  
{	
	unsigned int Distance0=0xffff;
	unsigned int txx;		
	
		if(SensorReadnByte(devid,(unsigned char *)&txx, 0x04,2)==1)	
			    Distance0=txx&0xffff;	
	return Distance0;	
}
