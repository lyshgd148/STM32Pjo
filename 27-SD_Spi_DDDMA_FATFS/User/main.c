#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Serial.h"
#include "MySD.h"
#include "ff.h"
#include "ST7789.h"
#include <string.h>
#include <stdio.h>

FATFS fs;        // 文件系统对象
FIL file;        // 文件对象
FRESULT res;     // 操作结果
UINT bw, br;     // 实际读写的字节数



int main(void)
{
	uint8_t i=0;
	
    Serial_Init();
	ST7789_Init();
    Delay_ms(500);
	printf("Serial ok!\r\n");
	
   

	while(1)
	{
		res = f_mount(&fs, "", 1);
		if (res != FR_OK)
		{
			printf("挂载失败！错误码: %d\r\n", res);
		}
		else
		{
			printf("文件系统挂载成功\r\n");
		}

		res = f_open(&file, "DATA.bin", FA_READ);
		if (res == FR_OK)
		{	
			
			while(f_eof(&file)==0)
			{	
				
				f_read(&file, ST7789_Buf, sizeof(ST7789_Buf), &br);
				LCD_Fill_DMA(0,i*30,240,i*30+30);
				i++;
				if(i>=8) i=0;
					
			}
			f_close(&file);
		}
		
		else
		{
			printf("打开文件失败！\r\n");
		}

		f_mount(NULL, "", 1);
		printf("文件卸载成功\r\n");
		}
}

