#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

extern char Serial_RxPacket[100];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_Send_String(char *String);
uint32_t Serial_Pow(uint32_t X,uint32_t Y);
uint8_t Serial_CalculateLength(uint32_t Number);
void Serial_SendNumber(uint32_t Number);
void USART1_IRQHandler(void);
void Serial_Printf(char *format,...);

#endif
