#ifndef _PAC9685_H
#define _PAC9685_H

void PAC9685_Write(uint8_t Addr,uint8_t Data);
uint8_t PAC9685_Read(uint8_t Addr);
void PCA9685_SetFrequcy(float freq);
void PCA9685_SetPWM(uint8_t num,uint32_t on,uint32_t off);
void PCA9685_SetAngle(uint8_t num,uint16_t angle);
void PAC9685_Init(void);

#endif
