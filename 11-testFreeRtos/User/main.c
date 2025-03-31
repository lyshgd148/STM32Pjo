#include "stm32f10x.h"                  // Device header
#include "LEDandKey.h"
#include "MyTask.h"

int main(void)
{
	TIM4_Config();
	LEDandKey_Init();
	

	freertos_start();
	
	while (1)
	{
		
	}
}
