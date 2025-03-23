#include "stm32f10x.h"                  // Device header
#include  "Delay.h"


void Key_Init(void){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IPD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
}

uint8_t Judge(void){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1){
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1);
		return 1;
	}
	return 0;
}
