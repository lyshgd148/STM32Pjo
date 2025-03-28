#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Serial.h"
#include "Delay.h"
#include "PCA9685Reg.h"
#include <math.h>


#define Servo_Motors_360  2.28


//#define PCA9685_Addr 0x80
//#define PCA9685_Model 0x00
//#define PCA9685_LED0_ON_L 0x06
//#define PCA9685_LED0_ON_H 0x07
//#define PCA9685_LED0_OFF_L 0x08
//#define PCA9685_LED0_OFF_H 0x09
//#define PCA9685_Pre 0xFE



void PAC9685_Write(uint8_t Addr,uint8_t Data){
	MyI2C_Strat();
	MyI2C_SendByte(PCA9685_Addr);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Addr);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	
	MyI2C_Stop();
}


uint8_t PAC9685_Read(uint8_t Addr){
	uint8_t Data;
	
	MyI2C_Strat();
	MyI2C_SendByte(PCA9685_Addr);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Addr);
	MyI2C_ReceiveAck();
	
	MyI2C_Strat();
	MyI2C_SendByte(PCA9685_Addr|0x01);
	MyI2C_ReceiveAck();
	
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void PCA9685_SetFrequcy(float freq){
	uint8_t prescale,oldmode,newmode;
	double prescalevalue;
	
	freq*=0.98;
	prescalevalue=25000000;
	prescalevalue=(prescalevalue/4096/freq)-1;
	prescale=floor(prescalevalue+0.5f);
	
	oldmode=PAC9685_Read(PCA9685_Model);
	newmode=(oldmode&0x7F)|0x10;
	
	PAC9685_Write(PCA9685_Model,newmode);
	PAC9685_Write(PCA9685_Pre,prescale);
	PAC9685_Write(PCA9685_Model,oldmode);
	Delay_ms(5);
	PAC9685_Write(PCA9685_Model,oldmode|0xa1);
}


void PCA9685_SetPWM(uint8_t num,uint32_t on,uint32_t off){
	MyI2C_Strat();
	MyI2C_SendByte(PCA9685_Addr);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(PCA9685_LED0_ON_L+num*4);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(on&0xFF);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(on>>8);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(off&0xFF);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(off>>8);
	MyI2C_ReceiveAck();
	
	MyI2C_Stop();
}

void PCA9685_SetAngle(uint8_t num,uint16_t angle){
	uint32_t off=0;
	off=(uint32_t)(103+angle*Servo_Motors_360);
	PCA9685_SetPWM(num,0,off);
}


void PAC9685_Init(void){
	uint8_t i;
	
	MyI2C_Init();
	PAC9685_Write(PCA9685_Model,0x00);
	PCA9685_SetFrequcy(50);
	
	for(i=0;i<16;i++){
		PCA9685_SetAngle(i,0);
	}
	
}
