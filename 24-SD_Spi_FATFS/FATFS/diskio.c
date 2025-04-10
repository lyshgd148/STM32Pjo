#include "ff.h" /* Obtains integer types */
#include "diskio.h"
#include "MySD.h"

/* Definitions of physical drive number for each drive */
#define DEV_SD 0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = RES_OK;

	switch (pdrv)
	{
	case DEV_SD:
		stat = STA_NOINIT;
		stat &= ~STA_NOINIT;
		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = RES_OK;

	switch (pdrv)
	{
	case DEV_SD:
		SD_Init();
		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	LBA_t sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	DRESULT res = RES_OK;
	int result;

	if (!count)
	{
		return RES_PARERR;
	}

	switch (pdrv)
	{
	case DEV_SD:
		result = SD_Read_Sector(sector, count, buff);
		if (result != 0)
		{
			res = RES_ERROR;
		}
		return res;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	LBA_t sector,	  /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT res = RES_OK;
	int result;

	if (!count)
	{
		return RES_PARERR;
	}

	switch (pdrv)
	{
	case DEV_SD:
		result = SD_Write_Sector(sector, count, (uint8_t *)buff);
		if (result != 0)
		{
			res = RES_ERROR;
		}
		return res;
	}

	return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;

	switch (pdrv)
	{
	case DEV_SD:
		switch (cmd)
		{
		case CTRL_SYNC:
		{
			SD_Sync_Data();
			res = RES_OK;
		}
		break;
		case CTRL_TRIM:
		{
			res = RES_OK;
		}
		break;
		case GET_SECTOR_COUNT:
		{
			*(int *)buff = SD_Get_SectorNum();
		}
		break;
		case GET_SECTOR_SIZE:
		{
			*(int *)buff = SD_Get_SectorSize();
		}
		break;
		case GET_BLOCK_SIZE:
		{
			*(int *)buff = 8;
		}
		break;
		}
		return res;
	}

	return RES_PARERR;
}

DWORD get_fattime(void)
{
	// 返回格式：bit31:25 年 -1980, bit24:21 月, bit20:16 日
	//          bit15:11 时, bit10:5 分, bit4:0 秒/2
	return ((DWORD)(2025 - 1980) << 25) // 年，从1980年开始
		   | ((DWORD)4 << 21)			// 月（4月）
		   | ((DWORD)10 << 16)			// 日（10号）
		   | ((DWORD)12 << 11)			// 时（12点）
		   | ((DWORD)0 << 5)			// 分（0分）
		   | ((DWORD)0 >> 1);			// 秒（0秒除以2）
}
