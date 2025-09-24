
#include "sys.h"
#include "global.h"
#include "flash.h"

void Flash_Save(void)
{
	  uint32_t DataBuffer[13];
		unsigned char i;
		uint32_t  Address;	
		
	  FLASH_Status FLASHStatus = FLASH_COMPLETE;
	  FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
		FLASHStatus =FLASH_ErasePage(0x0803e000);
	  DataBuffer[0]=0xaa;
		for(i=0;i<12;i++)	
			DataBuffer[i+1]=Canshu[i];
		
		i=0;	
		Address=0x0803e000;
		while(FLASHStatus == FLASH_COMPLETE&&i<13)
		{	
			FLASHStatus =FLASH_ProgramWord(Address, DataBuffer[i]);
			Address += 4;
			i++;
		}
//		FLASHStatus =FLASH_ProgramWord(Address, DistSHhortest);   //最长有效距离
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, DistSHLongest);   //最短有效距离
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, TestTWTime);          // 体温检测时间
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, YinsheONFlag);         //语言
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, T0Start);         //音量
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, T0end);    //显示语言
//    Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, L0Start);             //室温补偿值
//		Address += 4;		
//		FLASHStatus =FLASH_ProgramWord(Address, L0end);   //最长有效距离
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, room_temp);   //最短有效距离
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, Language );          //未用到
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, MaxHumanTemp);         //语言
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, Display_Language );         
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, RoomTemp );   
//    Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, yinliang );             //音量
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, Liangdu );             //亮度
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, password_0_true );             //密码
//		Address += 4;
//		FLASHStatus =FLASH_ProgramWord(Address, Liangdu_SR );             //密码
		
		FLASH_Lock();
		
}


void Flash_Read(void)
{                                                                                                                                                 
	uint32_t DataBuffer[13];
	unsigned char i;
		 
	 for(i=0;i<13;i++) 
	 DataBuffer[i] = *(__IO uint32_t *)(0x0803e000+4*i);
	 if(DataBuffer[0]!=0xaa)
		{
			Flash_Save();
			return;
		}		
			  		
//	 DistSHhortest = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 DistSHLongest = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 TestTWTime = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 YinsheONFlag=*(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 T0Start=*(__IO uint32_t *)(0x0803e000+4*i);
//   i++;
//	 T0end=*(__IO uint32_t *)(0x0803e000+4*i); 
//	 i++;
//	 L0Start = *(__IO uint32_t *)(0x0803e000+4*i); 
//	 i++;
//		L0end = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 room_temp  = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 Language  = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 MaxHumanTemp=*(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 Display_Language =*(__IO uint32_t *)(0x0803e000+4*i);
//   i++;
//	 RoomTemp =*(__IO uint32_t *)(0x0803e000+4*i); 
//	 i++;
//	 yinliang  = *(__IO uint32_t *)(0x0803e000+4*i); 
//	 i++;
//	 Liangdu = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++;
//	 password_0_true  = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++; 
//	 if(password_0_true > 999999)
//		    password_0_true = 999999;
//	 Liangdu_SR  = *(__IO uint32_t *)(0x0803e000+4*i);
//	 i++; 
//	for(i=0;i<12;i++)	
//	 Canshu[i]=DataBuffer[1+i];	
}


		
	


