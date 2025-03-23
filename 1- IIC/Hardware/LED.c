#include "stm32f10x.h"                  // Device header

void LED_Init(void){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void LED_off(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void LED_on(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

void Turn_LED(void){
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)==1)
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
}