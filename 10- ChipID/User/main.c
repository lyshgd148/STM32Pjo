#include "stm32f10x.h" // Device header
#include "Delay.h"
//#include "OLED.h"
//#include "OLED_Font.h"
#include "stdio.h"
#include "Serial.h"
#include "math.h"



int main(void)
{
	uint16_t Data;
	Serial_Init();
	Data=*((uint16_t *)(0x1FFFF7E0 ));

    while (1)
    {
		if (Serial_RxFlag == 1)
		{
			printf("Data:%d\n",Data);
			Serial_RxFlag = 0;
		}
    }
}