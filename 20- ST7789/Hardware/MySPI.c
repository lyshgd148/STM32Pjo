#include "stm32f10x.h"                  // Device header


#define SCL	 GPIO_Pin_5
#define SDA  GPIO_Pin_7
#define RES  GPIO_Pin_6
#define DC   GPIO_Pin_4
#define BLK  GPIO_Pin_3


void MySpi_SCL(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,SCL,BitValue);
}

void MySpi_SDA(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,SDA,BitValue);
}

void MySpi_RES(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,RES,BitValue);
}

void MySpi_DC(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,DC,BitValue);
}

void MySpi_BLK(uint8_t BitValue){
	GPIO_WriteBit(GPIOA,BLK,BitValue);
}


void MySPI_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

