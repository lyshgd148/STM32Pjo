#include "stm32f10x.h" // Device header
#include "MySD.h"
#include "MySPI.h"
#include "Delay.h"
#include "Serial.h"

#define BLOCK_SIZE 512
#define ENTER_SD_SECTOR_SIZE (ENTER_SD_BLOCK_SIZE)

// 卡类型
#define MMC 0x01
#define V1 0x02
#define V2 0x04
#define V2HC 0x06

uint8_t SD_Type;

uint8_t SD_Read_Write_Byte(uint8_t Data)
{
    uint8_t receive;
    receive = MySPI_SwapByte(Data);
    return receive;
}

uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1, retry;
    MySPI_CS_HIGH();

    SD_Read_Write_Byte(0xFF);
    MySPI_CS_LOW();
    do
    {
        retry = SD_Read_Write_Byte(0xFF);
    } while (retry != 0xFF);
    SD_Read_Write_Byte(cmd | 0x40);
    SD_Read_Write_Byte(arg >> 24);
    SD_Read_Write_Byte(arg >> 16);
    SD_Read_Write_Byte(arg >> 8);
    SD_Read_Write_Byte(arg);
    SD_Read_Write_Byte(crc);

    if (cmd == CMD12)
    {
        SD_Read_Write_Byte(0xFF);
    }
    do
    {
        r1 = SD_Read_Write_Byte(0xFF);
    } while (r1 & 0x80); // 等待响应
    return r1;
}

uint8_t SD_Read_Data(uint8_t *data, uint16_t len)
{
    uint8_t r1;
    MySPI_CS_LOW();
    do
    {
        r1 = SD_Read_Write_Byte(0xFF);
    } while (r1 != 0xFE);

    while (len--)
    {
        *data = SD_Read_Write_Byte(0xFF);
        data++;
    }
    SD_Read_Write_Byte(0xFF);
    SD_Read_Write_Byte(0xFF);
    return 0;
}

uint8_t SD_Send_Block_Data(uint8_t *data, uint8_t cmd)
{
    uint16_t i;
    uint8_t r1;

    do
    {
        r1 = SD_Read_Write_Byte(0xFF);
    } while (r1 != 0xFF);

    SD_Read_Write_Byte(cmd);

    if (cmd != 0xFD)
    {
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            SD_Read_Write_Byte(data[i]);
        }
        SD_Read_Write_Byte(0xFF);
        SD_Read_Write_Byte(0xFF);
        i = SD_Read_Write_Byte(0xFF);
        if ((i & 0x1F) != 0x05)
        {
            return 2;
        }
    }
    return 0;
}

uint8_t SD_Init(void)
{

    uint8_t r1, i;
    uint8_t buff[6] = {0};
    uint16_t retry;

    MySPI_Init();
    MySPI_CS_HIGH();
    for (retry = 0; retry < 40; retry++)
    {
        SD_Read_Write_Byte(0xFF);
    }
    MySPI_CS_LOW();
    do
    {
        r1 = SD_SendCmd(CMD0, 0, 0x95);
    } while (r1 != 0x01);
    SD_Type = 0;
    r1 = SD_SendCmd(CMD8, 0x1AA, 0X87);
    if (r1 == 0x01)
    {
        for (i = 0; i < 4; i++)
        {
            buff[i] = SD_Read_Write_Byte(0xFF);
        }
        if (buff[2] == 0x01 && buff[3] == 0xAA)
        {
            retry = 0xFFFE;
            do
            {
                SD_SendCmd(CMD55, 0, 0x01);
                r1 = SD_SendCmd(CMD41, 0x40000000, 0X01);
            } while (r1 && retry--);
            if (retry && SD_SendCmd(CMD58, 0, 0x01) == 0)
            {
                for (i = 0; i < 4; i++)
                {
                    buff[i] = SD_Read_Write_Byte(0xFF);
                }
                if (buff[0] & 0x40)
                {
                    SD_Type = 0x06;
                }
                else
                {
                    SD_Type = 0x04;
                }
            }
        }
    }
    else
    {
        SD_SendCmd(CMD55, 0, 0x01);
        r1 = SD_SendCmd(CMD41, 0, 0x01);
        if (r1 <= 1)
        {
            SD_Type = 0x02;
            retry = 0xFFFE;
            do
            {
                SD_SendCmd(CMD55, 0, 0x01);
                r1 = SD_SendCmd(CMD41, 0, 0x01);
            } while (r1 && retry--);
        }
        else
        {
            SD_Type = 0x01;
            retry = 0xFFFE;
            do
            {
                r1 = SD_SendCmd(CMD1, 0, 0x01);
            } while (r1 && retry--);
        }
        if (retry == 0 || SD_SendCmd(CMD16, 512, 0x01) != 0)
        {
            SD_Type = 0;
        }
    }
    MySPI_CS_HIGH();

    SPI_Cmd(SPI1, DISABLE);
    SPI1->CR1 &= ~(SPI_CR1_BR);
    SPI1->CR1 |= SPI_BaudRatePrescaler_4;
    SPI_Cmd(SPI1, ENABLE);

    return SD_Type;
}

uint8_t SD_Get_cid(uint8_t *cid_data)
{
    uint8_t r1;
    r1 = SD_SendCmd(CMD10, 0, 0x01);
    if (r1 == 0x00)
    {
        r1 = SD_Read_Data(cid_data, 16);
    }
    MySPI_CS_HIGH();
    if (r1 != 0)
    {
        return 1;
    }
    return 0;
}

uint8_t SD_Get_csd(uint8_t *csd_data)
{
    uint8_t r1;
    r1 = SD_SendCmd(CMD9, 0, 0x01);
    if (r1 == 0x00)
    {
        r1 = SD_Read_Data(csd_data, 16);
    }
    MySPI_CS_HIGH();
    if (r1 != 0)
    {
        return 1;
    }
    return 0;
}

uint32_t SD_Read_Sector(uint32_t sector, uint8_t num, uint8_t *data)
{
    uint8_t r1;
    if (SD_Type != V2HC)
    {
        sector <<= 9;
    }
    if (num == 1)
    {
        r1 = SD_SendCmd(CMD17, sector, 0x01);
        if (r1 == 0)
        {
            r1 = SD_Read_Data(data, BLOCK_SIZE);
        }
    }
    else
    {
        r1 = SD_SendCmd(CMD18, sector, 0x01);
        do
        {
            r1 = SD_Read_Data(data, BLOCK_SIZE);
            data += BLOCK_SIZE;
        } while (--num && r1 == 0);
        SD_SendCmd(CMD12, 0, 0x01);
    }
    MySPI_CS_HIGH();
    return 0;
}

uint32_t SD_Write_Sector(uint32_t sector, uint8_t num, uint8_t *data)
{
    uint8_t r1;
    if (SD_Type != V2HC)
    {
        sector <<= 9;
    }
    if (num == 1)
    {
        r1 = SD_SendCmd(CMD24, sector, 0x01);
        if (r1 == 0)
        {
            r1 = SD_Send_Block_Data(data, 0xFE);
        }
    }
    else
    {
        if (SD_Type != MMC)
        {
            SD_SendCmd(CMD55, 0, 0X01);
            SD_SendCmd(CMD23, num, 0X01);
        }
        r1 = SD_SendCmd(CMD25, sector, 0X01);
        if (r1 == 0)
        {
            do
            {
                r1 = SD_Send_Block_Data(data, 0xFC);
                data += BLOCK_SIZE;
            } while (--num && r1 == 0);
            r1 = SD_Send_Block_Data(0, 0xFD);
        }
    }
    MySPI_CS_HIGH();
    return 0;
}

uint32_t SD_Get_SectorNum(void)
{
    uint8_t csd[16];
    uint8_t n;
    uint32_t capacity;
    uint16_t csize;
    if (SD_Get_csd(csd) != 0)
    {
        return 0;
    }
    if ((csd[0] & 0xC0) == 0x40)
    {
        csize = csd[9] + ((u16)csd[8] << 8) + 1;
        capacity = (uint32_t)csize << 10;
    }
    else
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
        capacity = (u32)csize << (n - 9);
    }
    return capacity;
}

uint32_t SD_Get_SectorSize(void)
{
    return BLOCK_SIZE;
}

uint32_t SD_Get_BlockSize(void)
{
    return BLOCK_SIZE;
}

uint32_t SD_Sync_Data(void)
{
    MySPI_CS_LOW();
    do
    {
        Delay_ms(5);
    } while (SD_Read_Write_Byte(0xFF) != 0xFF);
    MySPI_CS_HIGH();
    return 0;
}
