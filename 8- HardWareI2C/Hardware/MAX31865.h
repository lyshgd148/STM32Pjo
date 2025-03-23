#ifndef _MAX31865_H
#define _MAX31865_H


void MAX31865_Init(void);
uint16_t Max31865_ReadRTD(void);
float MAX31865_GetTemperature(uint16_t Data);

#endif
