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
uint8_t SD_Buf[6002];



int main(void)
{
	
	
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


		res = f_open(&file, "DATA.txt", FA_READ);
		if (res == FR_OK)
		{	
			
			uint8_t t=0;
			memset(SD_Buf, 0, sizeof(SD_Buf));
			while(f_eof(&file)==0)
			{	
				
				char Data_High[3]={0};
				char Data_Low[3]={0};
				uint16_t i=0,k,j=0;
				f_read(&file, SD_Buf, sizeof(SD_Buf), &br);
				while(i<6000)
				{	
					if(SD_Buf[i]!=' ') 
					{	
						k=i;
						i%=5;
						if(i<2) Data_High[i]=SD_Buf[k];
						else Data_Low[i-2]=SD_Buf[k];
						i=k;
						
					}
					else
					{	
						sscanf(Data_High, "%hhx", ST7789_Buf+j);
						j++;
						sscanf(Data_Low, "%hhx", ST7789_Buf+j);
						j++;
					}
					i++;
				}
				LCD_Fill_DMA(0,t*5,240,t*5+5);
				
				t++;
				if(t>=48) t=0;
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

