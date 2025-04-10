#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "ST7789.h"
#include <math.h>



int main(void)
{
	uint8_t i,j;
	uint16_t Color=0;
	ST7789_Init();
	LCD_Clear(0x0000);
	Delay_ms(50);
	

	
    while (1)
	{
//		移动正弦波
//		for (j=0;j<240;j++)
//		{
//			for(i=0;i<240;i++)
//			{
//				LCD_DrawPoint(i,-120*sin((j+i)/240.0*3.14*2)+120,0xFFFF);
//			}
//			
//			for(i=0;i<240;i++)
//			{
//				LCD_DrawPoint(i,-120*sin((j+i)/240.0*3.14*2)+120,0x0000);
//			}
//		}
		
//		区域填充
		LCD_Fill(50,10,100,100,Color);
		LCD_Fill(50,101,100,200,65535-Color);
		LCD_Fill(101,10,150,101,32768+Color);
		LCD_Fill(101,101,150,200,32768-Color);
		Color+=10;
		
    }
}
