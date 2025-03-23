#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "AD.h"
#include "stdio.h"
#include "Serial.h"
#include "math.h"

extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
    char str[20];
    float volage;
    AD_Init();
    Serial_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"Volage:",6);
    OLED_Updata();


    while (1)
    {
        volage=AD_GetValue()/4095.0*6.6;
        sprintf(str,"%f",volage);
        OLED_ShowString(0,50,str,6);
        printf("%f\n",volage);
        OLED_Updata();
        Delay_ms(100);
    }
}
