#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "ST7789.h"
#include "Serial.h"
#include <math.h>

//计算帧数

double freq;
uint16_t Count,prescale=1000;
 






int main(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint8_t i,j;
	uint16_t Color=0;
	ST7789_Init();
	LCD_Clear(0x0000);
	Serial_Init();
	Delay_ms(50);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启 TIM2 时钟
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                // 自动重装载最大值
    TIM_TimeBaseStructure.TIM_Prescaler = prescale - 1;             // 72MHz / 36 = 2MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);  // 使能定时器

	
    while (1)
	{
//		移动正弦波
		for (j=0;j<240;j+=2)
		{
			for(i=0;i<240;i++)
			{
				LCD_DrawPoint(i,-120*sin((j+i)/240.0*3.14*2)+120,0xFFFF);
			}
			
			for(i=0;i<240;i++)
			{
				LCD_DrawPoint(i,-120*sin((j+i)/240.0*3.14*2)+120,0x0000);
			}
		}
		
//		区域填充
//		LCD_Fill(50,10,100,100,Color);
//		LCD_Fill(50,101,100,200,65535-Color);
//		LCD_Fill(101,10,150,101,32768+Color);
//		LCD_Fill(101,101,150,200,32768-Color);
//		Color+=10;
		
//		刷屏测试
//		TIM2->CNT=0;
//		LCD_Fill(0,0,240,240,0xFFFF);
//		LCD_Fill(0,0,240,240,0x0000);
//		LCD_Fill(0,0,240,240,0x001F);
//		LCD_Fill(0,0,240,240,0XF81F);
//		Count=TIM2->CNT;
//		freq=1.0/(72000000/prescale)*Count;
//		freq=1.0/freq*4;
//		printf("帧数:%f\r\n",freq);
    }
}
