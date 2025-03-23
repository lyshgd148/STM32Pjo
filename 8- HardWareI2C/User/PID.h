#ifndef _PID_H_
#define _PID_H_

#include <stdlib.h>
#include <string.h>

extern float Kp;
extern float Ki;
extern float Kd;
extern uint8_t PID_ChangeFlag;




void Receive_PID(void);
void PID_ShowInit(void);
void PID_ShowParameter(void);

#endif
