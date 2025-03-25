#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "AD.h"
#include "MAX31865.h"
#include "stdio.h"
#include "Serial.h"
#include "IOControl.h"
#include "PID.h"
#include "math.h"

extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
    char str[20];
    float volage,resistance;

	IOControl_Init();
    AD_Init();
    Serial_Init();
    MAX31865_Init();
    OLED_Init();
    PID_ShowInit();
    


    while (1)
    {
        Receive_PID();
        PID_ShowParameter();
        BatteryWaring();
        PID_Control();

    }
}