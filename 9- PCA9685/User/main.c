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
			if(Serial_RxPacket[4]=='1'){
			PCA9685_SetAngle(0,angle);
			printf("Angle1_OK!\n");
			}
			else if(Serial_RxPacket[4]=='2'){
			PCA9685_SetAngle(1,angle);
			printf("Angle2_OK!\n");
			}
			else if(Serial_RxPacket[4]=='3'){
			PCA9685_SetAngle(2,angle);
			printf("Angle3_OK!\n");
			}

			Serial_RxFlag = 0;
		}
    }
}