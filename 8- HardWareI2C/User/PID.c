#include "stm32f10x.h" // Device header
#include <stdlib.h>
#include <string.h>
#include "AD.h"
#include "PID.h"
#include "MAX31865.h"
#include "OLED.h"
#include "IOControl.h"
#include "MAX31865.h"
#include "Delay.h"
#include "Serial.h"

#define timeBase 200
#define Voltage_proportion 6.6335

float Kp = 20.36;
float Ki = 0.088;
float Kd = 10;
float voltage;
uint8_t voltageFlag = 0;
float temperatureSet = 100.0;
float temperatureNow;
int16_t timeHigh, timeLow;
extern char Serial_RxPacket[100];
extern uint8_t Serial_RxFlag;

void Receive_PID(void)
{
	if (Serial_RxFlag == 1)
	{
		float value;
		char *p = strchr(Serial_RxPacket, '=');
		if (p)
		{
			value = atof(p + 1);
		}
		if (Serial_RxPacket[0] == 'p')
		{
			Kp = value;
			printf("Kp_OK\n");
		}

		else if (Serial_RxPacket[0] == 'i')
		{
			Ki = value;
			printf("Ki_OK\n");
		}

		else if (Serial_RxPacket[0] == 'd')
		{
			Kd = value;
			printf("Kd_OK\n");
		}
		else if (Serial_RxPacket[0] == 't')
		{
			temperatureSet = value;
			printf("Kt_OK\n");
		}
		Serial_RxFlag = 0;
	}
}

void PID_ShowInit(void)
{
	OLED_ShowString(0, 0, "Kp=", 6);
	OLED_ShowString(0, 10, "Ki=", 6);
	OLED_ShowString(0, 20, "Kd=", 6);
	OLED_ShowString(0, 30, "TempSet:", 6);
	OLED_ShowString(0, 40, "TempNow:", 6);
	OLED_ShowString(0, 50, "voltage:", 6);
	OLED_Updata();
}

void PID_ShowParameter(void)
{
	char str[20];

	snprintf(str, sizeof(str), "%.4f", Kp);
	OLED_ShowString(18, 0, str, 6);
	snprintf(str, sizeof(str), "%.4f", Ki);
	OLED_ShowString(18, 10, str, 6);
	snprintf(str, sizeof(str), "%.4f", Kd);
	OLED_ShowString(18, 20, str, 6);
	snprintf(str, sizeof(str), "%.2f", temperatureSet);
	OLED_ShowString(50, 30, str, 6);

	temperatureNow = MAX31865_GetTemperature(Max31865_ReadRTD());
	printf("%.2f\n",temperatureNow);
	snprintf(str, sizeof(str), "%.2f", temperatureNow);
	OLED_ShowString(50, 40, str, 6);

	voltage = AD_GetValue() / 4095.0 * Voltage_proportion;
	snprintf(str, sizeof(str), "%.2f", voltage);
	OLED_ShowString(50, 50, str, 6);

	OLED_Updata();
}

void BatteryWaring(void)
{
	static uint16_t i = 0;
	if ((i < (600000 / timeBase)) && (voltageFlag == 0))
	{
		i++;
		if (voltage < 3.2)
		{
			OpenLED();
			voltageFlag = 1;
		}
	}
	else if ((i > (600000 / timeBase)) && (voltageFlag == 0))
	{
		i = 0;
	}
}

void PID(float temperatureNow)
{
	float u=0;
	static float error_last_last=0;
	static float error_last = 0;
	static float error_now = 0;
	// static double error_all=0;
	
	// /* 位置式PID 还是有点拉跨！ 位置式用了所有的误差和，待会试试增量式*/

	// error_now = temperatureSet - temperatureNow;
	// error_all+=error_now;
	// // if(error_all>=timeBase/Ki/5) error_all=timeBase/Ki/5;
	// // else if (error_all<=-timeBase/Ki/5) error_all=-timeBase/Ki/5;



	// timeHigh = Kp * error_now +Ki*error_all+ Kd * (error_now - error_last);

	// if (timeHigh > timeBase)
	// {
	// 	timeHigh = timeBase;
	// }
	// else if (timeHigh < 0)
	// 	timeHigh = 0;
	// timeLow = timeBase - timeHigh;
	// error_last = error_now;


	error_now = temperatureSet - temperatureNow;


	u = Kp * (error_now-error_last) +Ki*error_now+ Kd * (error_now - 2*error_last+error_last_last);
	timeHigh+=u;
	if (timeHigh > timeBase)
	{
		timeHigh = timeBase;
	}
	else if (timeHigh < 0)
		timeHigh = 0;
	timeLow = timeBase - timeHigh;

	error_last_last=error_last;
	error_last = error_now;
	
	
}

void PID_Control(void)
{

	PID(temperatureNow);
	OpenHeating();
	Delay_ms(timeHigh);
	CloseHeating();
	Delay_ms(timeLow);
}
