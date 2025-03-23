#include <stdlib.h>
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "Serial.h"


extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
	Serial_Init();
	
//	printf("Num=%d\r\n",666);	
	
	/*
	char String[100];
	sprintf(String,"Num=%d\r\n",666);
	Serial_SendString(String);
	*/
	
	/*
	Serial_Printf("Num=%d\r\n",666);
	*/
	
//	unsigned char i;
//	float num=32.5;
//	unsigned char *p;	
//	Delay_s(1);
//	p=(unsigned char *)&num;
//	for (i=0;i<4;i++){
//		Serial_SendByte(*(p+i));
//	}
	
	
    while (1)
    {
		printf("%f,%f\n",32.5,45.6);
		Delay_ms(100);
    }
}
