#ifndef _SPI_H
#define _SPI_H


#define MySPI_CS_HIGH()    GPIOA->BSRR = (1 << 4)
#define MySPI_CS_LOW()  GPIOA->BRR = (1 << 4)



void MySPI_Init(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
