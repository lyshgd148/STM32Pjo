#include "stm32f10x.h"                  // Device header
#include "LEDandKey.h"
#include "Delay.h"
#include "MyTask.h"
#include "Timer.h"
#include "Serial.h"

int main(void)
{
	TIM4_Config();
	Serial_Init();
	Timer_Init(10000,7200);
	LEDandKey_Init();

	freertos_start();
	
	while (1)
	{
		
	}
}
