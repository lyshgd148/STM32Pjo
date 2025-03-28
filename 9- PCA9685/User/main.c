#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "stdio.h"
#include "Serial.h"
#include <stdlib.h>
#include <string.h>
#include "PAC9685.h"
#include "math.h"



int main(void)
{
	Serial_Init();
    PAC9685_Init();

    while (1)
    {
		if (Serial_RxFlag == 1)
		{
			uint16_t angle;
			char *p = strchr(Serial_RxPacket, '=');
			if (p)
			{
				angle = (uint16_t)atoi(p + 1);
			}
			PCA9685_SetAngle(0,angle);
			printf("Angle_OK!\n");

			Serial_RxFlag = 0;
		}
    }
}