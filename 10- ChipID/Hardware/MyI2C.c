#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define SCL GPIO_Pin_8
#define SDA GPIO_Pin_9

void MyI2C_W_SCL(uint8_t BitValue){
	GPIO_WriteBit(GPIOB,SCL,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue){
	GPIO_WriteBit(GPIOB,SDA,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void){
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,SDA);
	Delay_us(10);
	return BitValue;
}


void MyI2C_Init(void){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	gpio.GPIO_Pin=SDA|SCL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	GPIO_SetBits(GPIOB,SCL|SDA);
}

void MyI2C_Strat(void){
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte){
	uint8_t i;
	for(i=0;i<8;i++){
		MyI2C_W_SDA(!!(Byte&(0x80>>i)));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void){
	uint8_t i;
	uint8_t Byte=0x00;
	MyI2C_W_SDA(1);
	for(i=0;i<8;i++){
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA()){Byte|=(0x80>>i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}


void MyI2C_SendAck(uint8_t AckBit){
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void){
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit=MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
