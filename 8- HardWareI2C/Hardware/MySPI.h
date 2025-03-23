#ifndef _SPI_H
#define _SPI_H

void MySPI_W_SS(uint8_t BitValue);

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);

uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
