#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Serial.h"
#include "W25Q64.h"





extern uint8_t OLED_DisplayBuf[8][128];


int main(void)
{	
	uint8_t i,SendData[200],ReceiveData[200];
	for(i=0;i<200;i++) 
	{
		SendData[i]=i;
	}


	
	W25Q64_Init();
	W25Q64_SectorErase(0x000000);
	W25Q64_Pageprogram(0x000000,SendData,200);
	W25Q64_ReadData(0x000000,ReceiveData,200);
	Serial_Init();
	
    while (1){
    	for(i=0;i<200;i++)
		{
			printf("Num:%d\n",ReceiveData[i]);
			Delay_ms(1);
		}
		Delay_ms(10);
    }
}
