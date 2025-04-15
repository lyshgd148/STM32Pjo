#ifndef _SPI_H
#define _SPI_H


#define RES_Clear()  GPIOA->BRR = GPIO_BRR_BR11
#define RES_Set()    GPIOA->BSRR = GPIO_BSRR_BS11
#define DC_Clear()   GPIOA->BRR = GPIO_BRR_BR12
#define DC_Set()     GPIOA->BSRR = GPIO_BSRR_BS12
#define BLK_Clear()  GPIOA->BRR = GPIO_BRR_BR3
#define BLK_Set()    GPIOA->BSRR = GPIO_BSRR_BS3

#define MySPI_CS_HIGH()    GPIOA->BSRR = (1 << 4)
#define MySPI_CS_LOW()     GPIOA->BRR = (1 << 4)




void MySPI1_Init(void);
uint8_t MySPI1_SwapByte(uint8_t ByteSend);

void MySPI2_Init(void);


#endif
