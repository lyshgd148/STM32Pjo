#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(void){
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Pin=GPIO_Pin_9;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_IPU;
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	usart.USART_BaudRate=115200;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity=USART_Parity_No;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&usart);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
	nvic.NVIC_IRQChannel=USART1_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=1;
	nvic.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvic);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Array[i]);
	}
}

void Serial_Send_String(char *String){
	uint16_t i;
	for(i=0;String[i]!='\0';i++){
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y){
	uint32_t result=X;
	while (Y--){
		result*=X;
	}
	return result;
}

uint8_t Serial_CalculateLength(uint32_t Number){
	uint8_t result=0;
	if(Number==0) return 1;
	while(Number){
		result++;
		Number/=10;
	}
	return result;
}

void Serial_SendNumber(uint32_t Number){
	uint8_t i,Length;
	Length=Serial_CalculateLength(Number);
	for(i=0;i<Length;i++){
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

int fputc(int ch,FILE*f){
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format,...){
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_Send_String(String);	
}


void USART1_IRQHandler(void){
	
	static uint8_t RxState=0;
	static uint8_t pRxPacket=0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		uint8_t RxData=USART_ReceiveData(USART1);
		if(RxState==0){
			if(RxData=='A'&& Serial_RxFlag==0){
				RxState=1;
				pRxPacket=0;
			}
		}
		else if(RxState==1){
			if(RxData=='\r') RxState=2;
			else{
				Serial_RxPacket[pRxPacket]=RxData;
				pRxPacket++;
			}
		}
		else if(RxState==2){
			if(RxData=='\n'){
				RxState=0;
				Serial_RxPacket[pRxPacket]='\0';
				Serial_RxFlag=1;
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

