#ifndef _MYDMA_H
#define _MYDMA_H


extern uint8_t dummy;

void My_DMA_Init1(void);
void My_DMA_TransferAndReceive1(uint8_t* SendData,uint8_t* ReceiveData,uint16_t times);

	
void My_DMA_Init2(uint32_t Add_Memory,uint16_t BufferSize);
void My_DMA_Transfer2(uint8_t* data,uint16_t times);

#endif
