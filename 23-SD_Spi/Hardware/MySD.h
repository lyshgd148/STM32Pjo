#ifndef _MYSD_H
#define _MYSD_H


#define CMD0        0//卡复位
#define CMD1        1
#define CMD8        8//命令8 ，SEND_IF_COND
#define CMD9        9//命令9 ，读CSD数据
#define CMD10       10//命令10，读CID数据
#define CMD12       12//命令12，停止数据传输
#define CMD16       16//命令16，设置SectorSize 应返回0x00
#define CMD17       17//命令17，读sector
#define CMD18       18//命令18，读Multi sector
#define CMD23       23//命令23，设置多sector写入前预先擦除N个block
#define CMD24       24//命令24，写sector
#define CMD25       25//命令25，写Multi sector
#define CMD41       41//命令41，应返回0x00
#define CMD55       55//命令55，应返回0x01
#define CMD58       58//命令58，读OCR信息
#define CMD59       59//命令59，使能/禁止CRC，应返回0x00

extern uint8_t SD_Type;




uint8_t SD_Read_Write_Byte(uint8_t Data);
uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_Read_Data(uint8_t *data, uint16_t len);
uint8_t SD_Send_Block_Data(uint8_t *data, uint8_t cmd);
uint8_t SD_Init(void);


uint8_t SD_Get_cid(uint8_t *cid_data);
uint8_t SD_Get_csd(uint8_t *csd_data);
uint8_t SD_Read_Sector(uint32_t sector, uint8_t num, uint8_t *data);
uint8_t SD_Write_Sector(uint32_t sector, uint8_t num, uint8_t *data);
uint32_t SD_Get_SectorNum(void);
uint32_t SD_Get_SectorSize(void);
uint32_t SD_Get_BlockSize(void);
uint8_t b_sd_sync_data(void) ;
#endif
