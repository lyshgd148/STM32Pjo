#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "Serial.h"
#include "PID.h"


extern uint8_t OLED_DisplayBuf[8][128];


int main(void)
{	
	OLED_Init();
	Serial_Init();	
	PID_ShowInit();
	
	
	
    while (1)
    {
		Receive_PID();
		PID_ShowParameter();
    }
}
