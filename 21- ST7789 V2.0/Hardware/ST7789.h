#ifndef _ST7789_H
#define _ST7789_H

void ST7789_Write_Bus(uint8_t Data);
void ST7789_Write_Data8(uint8_t Data);
void ST7789_Write_Cmd8(uint8_t Cmd);
void ST7789_Write_Cmd_Data16(uint8_t Cmd,uint16_t Data);
void ST7789_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ST7789_Init(void);


void LCD_Clear(uint16_t Color);
void  LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t Color);
void LCD_Fill(uint16_t x_Start,uint16_t y_Strat,uint16_t x_End,uint16_t y_End,uint16_t Color);

#endif
