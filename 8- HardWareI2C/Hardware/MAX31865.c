#include "stm32f10x.h"
#include "MySPI.h"
#include "MAX31865ADDRES.h"
#include <math.h>

#define MAX31865_PT1000_Resistance_Ref 4280
#define MAX31865_pt100_Resistance_Ref 430

#define MAX31865_2_4_Line 0xC1
#define MAX31865_3_Line 0xD1

#define MAX31865_DUMMY 0xFF

#define A 3.9083e-3

void MAX31865_Init(void)
{
    MySPI_Init();
    MySPI_Start();
    MySPI_SwapByte(MAX31865_CONFIGURATION_W);
    MySPI_SwapByte(MAX31865_2_4_Line);
    MySPI_Stop();
}

uint16_t Max31865_ReadRTD(void)
{
    uint16_t H_Data, L_Data, Data;

    MySPI_Start();
    MySPI_SwapByte(MAX31865_RTD_MSBS_R);
    H_Data = MySPI_SwapByte(MAX31865_DUMMY);
    L_Data = MySPI_SwapByte(MAX31865_DUMMY);
    MySPI_Stop();
    Data = (H_Data << 7) | (L_Data >> 1);
    return Data;
}

float MAX31865_GetTemperature(uint16_t Data)
{   
    float resistance,result;
    resistance = 1.0 * MAX31865_PT1000_Resistance_Ref * Data / 32768.0;
    result=(resistance-1000)/(A*1000.0);


    return result;
    /*

     */
}
