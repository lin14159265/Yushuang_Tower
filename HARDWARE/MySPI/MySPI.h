#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
unsigned char MySPI_SwapByte(unsigned char ByteSend);

#endif
