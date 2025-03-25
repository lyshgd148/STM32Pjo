#ifndef _PID_H_
#define _PID_H_

#include <stdlib.h>
#include <string.h>




extern float Kp;
extern float Ki;
extern float Kd;
extern float voltage;
extern uint8_t voltageFlag;
extern float temperatureSet;
extern float temperatureNow;
extern int16_t timeHigh;
extern int16_t timeLow;



void Receive_PID(void);
void PID_ShowInit(void);
void PID_ShowParameter(void);
void BatteryWaring(void);
void PID(float temperatureNow);
void PID_Control(void);

#endif
