#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MPU6050.h"
#include "Serial.h"
#include "OLED.h"
#include "MPU6050Reg.h"

int16_t Ax,Ay,Az,Gx,Gy,Gz;


int main(void){
	OLED_Init();
	MPU6050_Init();
	
	
	while(1){
		MPU6050_GetData(&Ax,&Ay,&Az,&Gx,&Gy,&Gz);
		OLED_ShowHexNum(4,4,MPU6050_ReadReg(MPU6050_WHO_AM_I),2);
		OLED_ShowSignedNum(1,1,Ax,4);
		OLED_ShowSignedNum(2,1,Ay,4);
		OLED_ShowSignedNum(3,1,Az,4);
		OLED_ShowSignedNum(1,8,Gx,4);
		OLED_ShowSignedNum(2,8,Gy,4);
		OLED_ShowSignedNum(3,8,Gz,4);
	}
}
