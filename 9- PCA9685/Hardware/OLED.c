#include "stm32f10x.h"
#include "MyI2C.h"
#include "Delay.h"
#include "OLED.h"
#include "string.h"


uint8_t OLED_DisplayBuf[8][128];

extern const uint8_t OLED_F6x8[][6];
extern const uint8_t OLED_F8x16[][16];
typedef struct
{
    char Index[4];
    uint8_t Data[32];
} ChineseCell_t;

extern const ChineseCell_t OLED_CF16x16[];

void OLED_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	
	{
		Timeout --;										
		if (Timeout == 0)								
		{
			break;										
		}
	}
}

void OLED_WriteCommand(uint8_t command)
{
    // MyI2C_Strat();
    // MyI2C_SendByte(0x78);
    // MyI2C_ReceiveAck();
    // MyI2C_SendByte(0X00);
    // MyI2C_ReceiveAck();
    // MyI2C_SendByte(command);
    // MyI2C_ReceiveAck();
    // MyI2C_Stop();

    I2C_GenerateSTART(I2C2, ENABLE);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C2, 0x00);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    I2C_SendData(I2C2, command);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTOP(I2C2, ENABLE);		
}

void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;
    // MyI2C_Strat();
    // MyI2C_SendByte(0x78);
    // MyI2C_ReceiveAck();
    // MyI2C_SendByte(0X40);
    // MyI2C_ReceiveAck();
    // for (i = 0; i < Count; i++)
    // {
    //     MyI2C_SendByte(Data[i]);
    //     MyI2C_ReceiveAck();
    // }
    // MyI2C_Stop();
    
    I2C_GenerateSTART(I2C2, ENABLE);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter);
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C2, 0x40);
    for(i=0;i<Count;i++){
        OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
        I2C_SendData(I2C2, Data[i]);
    }
    OLED_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTOP(I2C2, ENABLE);
}

void OLED_Init(void)
{
    // MyI2C_Init();

    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//开启I2C2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PB10和PB11引脚初始化为复用开漏输出

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//模式，选择为I2C模式
	I2C_InitStructure.I2C_ClockSpeed = 400000;				//时钟速度，选择为200KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//时钟占空比，选择Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//应答，选择使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//应答地址，选择7位，从机模式下才有效
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//自身地址，从机模式下才有效
	I2C_Init(I2C2, &I2C_InitStructure);						//将结构体变量交给I2C_Init，配置I2C2
	I2C_Cmd(I2C2, ENABLE);	

    Delay_ms(100);
    OLED_WriteCommand(0xAE);

    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0xA1);

    OLED_WriteCommand(0xC8);

    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);

    OLED_WriteCommand(0xA6);

    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);

    Delay_ms(100);
}

void OLED_SetCursor(uint8_t X, uint8_t Page)
{
    OLED_WriteCommand(0xB0 | Page);
    OLED_WriteCommand(0x0F & X);
    OLED_WriteCommand(0x10 | (X >> 4));
}

void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 128; i++)
        {
            OLED_DisplayBuf[j][i] = 0x00;
        }
    }
}

void OLED_Updata(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_SetCursor(0, i);
        OLED_WriteData(OLED_DisplayBuf[i], 128);
    }
}

void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t i, j;
    for (j = Y; j < Y + Height; j++)
    {
        for (i = X; i < X + Width; i++)
        {
            OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));
        }
    }
}

void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Img)
{
    uint8_t i, j;
    OLED_ClearArea(X, Y, Width, Height);
    for (j = 0; j < (Height - 1) / 8 + 1; j++)
    {
        for (i = X; i < X + Width; i++)
        {
            OLED_DisplayBuf[Y / 8 + j][i] |= (Img[Width * j + i - X] << (Y % 8));
            OLED_DisplayBuf[Y / 8 + j + 1][i] |= (Img[Width * j + i - X] >> (8 - Y % 8));
        }
    }
}

void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
    if (FontSize == 6)
    {
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
    }
    else if (FontSize == 8)
    {
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    }
}

void OLED_ShowString(uint8_t X, uint8_t Y, char *string, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; string[i] != '\0'; i++)
    {
        OLED_ShowChar(X + i * FontSize, Y, string[i], FontSize);
    }
}

void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
    char SigleChinese[4] = {0};
    uint8_t i, k, j = 0;

    for (i = 0; Chinese[i] != '\0'; i++)
    {
        SigleChinese[j] = Chinese[i];
        j++;
        if (j >= 3)
        {
            j = 0;
            for (k = 0; strcmp(OLED_CF16x16[k].Index, "") != 0; k++)
            {
                if (strcmp(OLED_CF16x16[k].Index, SigleChinese) == 0)
                {
                    break;
                }
            }
            OLED_ShowImage(X + i / 3 * 16, Y, 16, 16, OLED_CF16x16[k].Data);
        }
    }
}

void OLED_DrawPoint(uint8_t X,uint8_t Y)
{
    OLED_DisplayBuf[Y/8][X]|=(0x01<<(Y%8));
}

uint8_t OLED_GetPoint(uint8_t X,uint8_t Y){
    if(OLED_DisplayBuf[Y/8][X]&(0x01<<(Y%8))) return 1;    
    return 0;
}
