#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "AD.h"
#include "MAX31865.h"
#include "stdio.h"
#include "Serial.h"
#include "IOControl.h"
#include "math.h"

extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
    char str[20];
    float volage,resistance;
    // uint8_t i, j;
	IOControl_Init();
    AD_Init();
    Serial_Init();
    MAX31865_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"Temp:",6);
    OLED_Updata();
	
	OpenLED();
	OpenHeating();

    while (1)
    {
        // volage=AD_GetValue()/4095.0*6.6;
        // sprintf(str,"%.2f",volage);
        // OLED_ShowString(0,50,str,6);
        // printf("%f\n",volage);
        // OLED_Updata();
        // Delay_ms(100);

    //     for(j=0;j<64;j++){
	// 		for(i=0;i<128;i++){OLED_DrawPoint(i,(uint8_t)(32*(-1)*cos((i+j*2)/128.0*2*3.14)+32));}
	// 		OLED_Updata();
	// 		Delay_ms(50);
	// 		OLED_Clear();
    // }

    resistance=MAX31865_GetTemperature(Max31865_ReadRTD());
        sprintf(str,"%.2f",resistance);
        OLED_ShowString(50,0,str,6);
        printf("%.2f\n",resistance);
       
		volage=AD_GetValue()/4095.0*6.6;
        sprintf(str,"%.2f",volage);
        OLED_ShowString(0,50,str,6);
        printf("%.2f\n",volage);
        OLED_Updata();
		
			
        if(resistance>100) CloseHeating();
		else OpenHeating();
		OLED_Updata();
    }
}