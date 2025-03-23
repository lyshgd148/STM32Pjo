#include "stm32f10x.h"                  // Device header
#include <stdlib.h>
#include <string.h>
#include "MAX31865.h"
#include "OLED.h"
#include "Serial.h"

float Kp=0.05;
float Ki=0.01;
float Kd=0.02;
uint8_t PID_ChangeFlag;

extern char Serial_RxPacket[100];
extern uint8_t Serial_RxFlag;

void Receive_PID(void){
	if(Serial_RxFlag==1){
		float value;
		char *p=strchr(Serial_RxPacket,'=');
		PID_ChangeFlag=1;
		if(p){
			value =atof(p+1);
		}
		if(Serial_RxPacket[0]=='p') 	
		{
			Kp=value;
			printf("Kp_OK\n");
		}
		
		else if(Serial_RxPacket[0]=='i')
		{
			Ki=value;
			printf("Ki_OK\n");
		}
		
		else if(Serial_RxPacket[0]=='d')
		{
			Kd=value;
			printf("Kd_OK\n");
		}
		Serial_RxFlag=0;
	}
}

void PID_ShowInit(void){
	char str[10];
	OLED_ShowString(0,0,"Kp=",6);
	OLED_ShowString(0,15,"Ki=",6);
	OLED_ShowString(0,30,"Kd=",6);
	snprintf(str, sizeof(str), "%.4f", Kp);
	OLED_ShowString(18,0,str,6);
	snprintf(str, sizeof(str), "%.4f", Ki);
	OLED_ShowString(18,15,str,6);
	snprintf(str, sizeof(str), "%.4f", Kd);
	OLED_ShowString(18,30,str,6);
	OLED_Updata();
}

void PID_ShowParameter(void){
	if(PID_ChangeFlag==1){
		char str[10];
		OLED_ShowString(0,0,"Kp=",6);
		OLED_ShowString(0,15,"Ki=",6);
		OLED_ShowString(0,30,"Kd=",6);
		snprintf(str, sizeof(str), "%.4f", Kp);
		OLED_ShowString(18,0,str,6);
		snprintf(str, sizeof(str), "%.4f", Ki);
		OLED_ShowString(18,15,str,6);
		snprintf(str, sizeof(str), "%.4f", Kd);
		OLED_ShowString(18,30,str,6);
		OLED_Updata();
		PID_ChangeFlag=0;
	}
	
}

