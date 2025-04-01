#include "stm32f10x.h"                  // Device header
#include "LEDandKey.h"
#include "MyTask.h"
#include "Delay.h"
#include "Serial.h"

int main(void)
{
	TIM4_Config();
	Serial_Init();
	LEDandKey_Init();

	freertos_start();
	
	while (1)
	{
		
	}
}
