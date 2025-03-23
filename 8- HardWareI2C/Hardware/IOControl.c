#include "stm32f10x.h" // Device header

void IOControl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void OpenHeating(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void CloseHeating(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

void OpenLED(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void CloaeLED(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}
