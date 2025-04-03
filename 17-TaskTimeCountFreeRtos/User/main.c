#include "stm32f10x.h"                  // Device header
#include "LEDandKey.h"
#include "MyTask.h"
#include "Serial.h"
#include "Delay.h"
#include "Timer.h"

int main(void)
{
	TIM4_Config();
	LEDandKey_Init();
	Serial_Init();
	Timer_Init(10,72);
	Delay_ms(500);
	
	freertos_start();
	
	while (1)
	{
		
	}
}
