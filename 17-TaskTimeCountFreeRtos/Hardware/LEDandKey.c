#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "FreeRTOS.h"

void LEDandKey_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED1_ON(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
}

void LED1_OFF(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
}

void LED2_ON(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}

void LED2_OFF(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
}

uint8_t GetKey(void)
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
    {
        // vTaskDelay(10);
        Delay_ms(10);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
        {
            return 1;
        }
    }
    return 0;
}