#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MySPI.h"
#include "MyDMA.h"


#define LCD_W 240
#define LCD_H 240


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝
#define LIGHTBLUE      	 0X7D7C	//浅蓝  
#define GRAYBLUE       	 0X5458 //灰蓝
 
#define LIGHTGREEN     	 0X841F //浅绿
#define LGRAY 			 0XC618 //浅灰

#define LGRAYBLUE        0XA651 //浅灰蓝
#define LBBLUE           0X2B12 //浅棕蓝


uint8_t ST7789_Buf[14400];

void ST7789_Write_Bus(uint8_t Data)
{
	SPI_I2S_SendData(SPI2,Data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}


void ST7789_Write_Data8(uint8_t Data)
{
	DC_Set();
	SPI_I2S_SendData(SPI2,Data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}

void ST7789_Write_Data16(uint16_t Data)
{
	DC_Set();
	SPI_I2S_SendData(SPI2,Data>>8);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2,Data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}

void ST7789_Write_Cmd8(uint8_t Cmd)
{
	DC_Clear();
	ST7789_Write_Bus(Cmd);
}

void ST7789_Write_Cmd_Data16(uint8_t Cmd,uint16_t Data)
{
	ST7789_Write_Cmd8(Cmd);
	ST7789_Write_Data16(Data);
}

void ST7789_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	ST7789_Write_Cmd8(0x2a);
	ST7789_Write_Data16(x1);
	ST7789_Write_Data16(x2);
	
	ST7789_Write_Cmd8(0x2b);
	ST7789_Write_Data16(y1);
	ST7789_Write_Data16(y2);

	ST7789_Write_Cmd8(0x2C);
}


void ST7789_Init(void)
{
	MySPI2_Init();
	My_DMA_Init2((uint32_t)ST7789_Buf,14400);
	
	RES_Clear();
	Delay_ms(50);
	RES_Set();
	Delay_ms(50);
	BLK_Set();
	
	
	ST7789_Write_Cmd8(0x36); 
	ST7789_Write_Data8(0x00);

	ST7789_Write_Cmd8(0x3A); 
	ST7789_Write_Data8(0x05);

	ST7789_Write_Cmd8(0xB2);
	ST7789_Write_Data8(0x0C);
	ST7789_Write_Data8(0x0C);
	ST7789_Write_Data8(0x00);
	ST7789_Write_Data8(0x33);
	ST7789_Write_Data8(0x33);

	ST7789_Write_Cmd8(0xB7); 
	ST7789_Write_Data8(0x35);  

	ST7789_Write_Cmd8(0xBB);
	ST7789_Write_Data8(0x19);

	ST7789_Write_Cmd8(0xC0);
	ST7789_Write_Data8(0x2C);

	ST7789_Write_Cmd8(0xC2);
	ST7789_Write_Data8(0x01);

	ST7789_Write_Cmd8(0xC3);
	ST7789_Write_Data8(0x12);   

	ST7789_Write_Cmd8(0xC4);
	ST7789_Write_Data8(0x20);  

	ST7789_Write_Cmd8(0xC6); 
	ST7789_Write_Data8(0x0F);    

	ST7789_Write_Cmd8(0xD0); 
	ST7789_Write_Data8(0xA4);
	ST7789_Write_Data8(0xA1);

	ST7789_Write_Cmd8(0xE0);
	ST7789_Write_Data8(0xD0);
	ST7789_Write_Data8(0x04);
	ST7789_Write_Data8(0x0D);
	ST7789_Write_Data8(0x11);
	ST7789_Write_Data8(0x13);
	ST7789_Write_Data8(0x2B);
	ST7789_Write_Data8(0x3F);
	ST7789_Write_Data8(0x54);
	ST7789_Write_Data8(0x4C);
	ST7789_Write_Data8(0x18);
	ST7789_Write_Data8(0x0D);
	ST7789_Write_Data8(0x0B);
	ST7789_Write_Data8(0x1F);
	ST7789_Write_Data8(0x23);

	ST7789_Write_Cmd8(0xE1);
	ST7789_Write_Data8(0xD0);
	ST7789_Write_Data8(0x04);
	ST7789_Write_Data8(0x0C);
	ST7789_Write_Data8(0x11);
	ST7789_Write_Data8(0x13);
	ST7789_Write_Data8(0x2C);
	ST7789_Write_Data8(0x3F);
	ST7789_Write_Data8(0x44);
	ST7789_Write_Data8(0x51);
	ST7789_Write_Data8(0x2F);
	ST7789_Write_Data8(0x1F);
	ST7789_Write_Data8(0x1F);
	ST7789_Write_Data8(0x20);
	ST7789_Write_Data8(0x23);

	ST7789_Write_Cmd8(0x21); 

	ST7789_Write_Cmd8(0x11); 
	//Delay (120); 

	ST7789_Write_Cmd8(0x29); 
	
}


void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;
	ST7789_Address_Set(0,0,LCD_W-1,LCD_W-1);
	for(i=0;i<LCD_W;i++)
	{
		for(j=0;j<LCD_H;j++)
		{
			ST7789_Write_Data16(Color);
		}
	}
}

void  LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t Color)
{
	ST7789_Address_Set(x,y,x,y);
	ST7789_Write_Data16(Color);
}

void LCD_Fill(uint16_t x_Start,uint16_t y_Strat,uint16_t x_End,uint16_t y_End,uint16_t Color)
{
	uint16_t i,j;
	ST7789_Address_Set(x_Start,y_Strat,x_End,y_End);
	for(i=x_Start;i<x_End;i++)
	{
		for(j=y_Strat;j<y_End;j++)
		{
			ST7789_Write_Data16(Color);
		}
	}
}

void LCD_Fill_DMA(uint16_t x_Start,uint16_t y_Strat,uint16_t x_End,uint16_t y_End)
{
	ST7789_Address_Set(x_Start,y_Strat,x_End-1,y_End-1);
	DC_Set();
	My_DMA_Transfer2(ST7789_Buf,14400);
}
