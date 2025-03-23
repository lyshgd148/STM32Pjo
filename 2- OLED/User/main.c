#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "math.h"

extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
    uint8_t i,j;
    OLED_Init();
    OLED_Clear();

    while (1)
    {
		for(j=0;j<64;j++){
			for(i=0;i<128;i++){OLED_DrawPoint(i,(uint8_t)(32*(-1)*cos((i+j*2)/128.0*2*3.14)+32));}
			OLED_Updata();
			Delay_ms(50);
			OLED_Clear();
		}
        
    }
}
