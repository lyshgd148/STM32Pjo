#include "stm32f10x.h"                  // Device header



/*
DMA1:
CH1->ADC1   TIM2_CH3  TIM4_CH1
CH2->SPI1_RX   USART3_TX   TIM1_CH1   TIM2_UP  TIM3_CH3
CH3->SPI1_TX   USART3_RX   TIM1_CH2   TIM3_CH4  TIM3_UP
CH4->SPI/IICS2_RX    USART1_TX   IIC2_TX  TIM1_TX4  TIM1_TRIG   TIM1_COM    TIM4_CH2
CH5->SPI/IICS2_TX    USART1_RX   IIC2_RX  TIM1_UP   TIM2_CH1    TIM4_CH3
CH6->USART2_RX     IIC1_TX   TIM1_CH3    TIM3_CH1   TIM3_TRIG
CH7->USART2_TX     IIC1_RX     TIM2_CH2  TIM2_CH4   TIM4_UP

DMA2
CH1->SPI/IICS3_RX   TIM5_CH4    TIM5_TRIG   TIM8_CH3    TIM8_UP
CH2->SPI/IICS3_TX   TIM5_CH3    TIM5_UP     TIM8_CH4    TIM8_TRIG   TIM8_COM
CH3->UART4_RX   TIM6_UP/DAC_CH1     TIM8_CH1    
CH4->SDIO   TIM5_CH2    TIM7_UP/DAC_CH2
CH5->ADC3   UART4_TX    TIM5_CH1    TIM8_CH2
*/

uint8_t dummy = 0xFF;

//SD的DMA初始化 SPI1_RX
void My_DMA_Init1(void)
{
	DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // RX - SPI1->DR --> data[]
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;  // 暂不设置
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 0;      // 暂不设置
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    // TX - dummy 0xFF --> SPI1->DR
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&dummy;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    // 启用SPI DMA
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
}

void My_DMA_TransferAndReceive1(uint8_t* SendData,uint8_t* ReceiveData,uint16_t times)
{

    DMA_Cmd(DMA1_Channel2, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);

	
    DMA1_Channel2->CMAR = (uint32_t)ReceiveData;
    DMA1_Channel3->CMAR = (uint32_t)SendData;
	DMA_SetCurrDataCounter(DMA1_Channel2, times);
	DMA_SetCurrDataCounter(DMA1_Channel3, times);
	

    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

	
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC3));
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));

    DMA_ClearFlag(DMA1_FLAG_TC2 | DMA1_FLAG_TC3);
}




//ST7789的DMA初始化 SPI2_TX
void My_DMA_Init2(uint32_t Add_Memory,uint16_t BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = Add_Memory;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//once
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
}


void My_DMA_Transfer2(uint8_t* data,uint16_t times)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel5, times);
	DMA1_Channel5->CMAR = (uint32_t)data;
	DMA_Cmd(DMA1_Channel5, ENABLE);

	while(DMA_GetFlagStatus(DMA1_FLAG_TC5)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	
}

