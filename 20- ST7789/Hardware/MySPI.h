#ifndef _SPI_H
#define _SPI_H

void MySpi_SCL(uint8_t BitValue);
void MySpi_SDA(uint8_t BitValue);
void MySpi_RES(uint8_t BitValue);
void MySpi_DC(uint8_t BitValue);
void MySpi_BLK(uint8_t BitValue);

void MySPI_Init(void);



#endif
