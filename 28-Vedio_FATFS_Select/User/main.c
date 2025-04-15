#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Serial.h"
#include "MySD.h"
#include "ff.h"
#include "ST7789.h"
#include <string.h>
#include <stdio.h>


void Vedio_Play(char name[]);
void ListFile(const char *path);

FATFS fs;        // 文件系统对象
FIL file;        // 文件对象
FRESULT res;     // 操作结果
FRESULT res1;     // 打开关闭文件操作结果
UINT bw, br;     // 实际读写的字节数

uint8_t file_state=0;

int main(void)
{
    Serial_Init();
	ST7789_Init();
    Delay_ms(500);
	
	printf("Serial ok!\r\n");
	res = f_mount(&fs, "", 1);
	if (res != FR_OK)
	{
		printf("挂载失败！错误码: %d\r\n", res);
	}
	else
	{
		printf("文件系统挂载成功\r\n");
	}

	ListFile("/");

	while(1)
	{
		if (Serial_RxFlag==1)
		{
			char name[20];
			Serial_RxFlag=0;
			sscanf(Serial_RxPacket,"%*[^=]=%s",name);
			if(strcmp(name,"DELAY")==0)
			{
				f_mount(NULL, "", 1);
				printf("文件卸载成功\r\n");
			}
			else if(strcmp(name,"LOAD")==0)
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
			}
			else
			{
				Vedio_Play(name);
			}
			
			
		}
		
	}
}



void Vedio_Play(char name[])
{
	uint8_t i=0;
	
	
	printf("name:%s\r\n",name);
	res1 = f_open(&file,name, FA_READ);
	if (res1 == FR_OK)
	{	
		file_state=1;
		while(f_eof(&file)==0)
		{	
			
			f_read(&file, ST7789_Buf, sizeof(ST7789_Buf), &br);
			LCD_Fill_DMA(0,i*30,240,i*30+30);
			i++;
			if(i>=8) i=0;
			if (file_state==0) break;
				
		}
		f_close(&file);
		
	}
	
	else
	{
		printf("打开文件失败！\r\n");
	}
}

void ListFile(const char *path)
{
	FRESULT res;
    DIR dir;            // 目录对象
    FILINFO fileinfo;   // 文件信息结构体

   
    res = f_opendir(&dir, path);
    if (res != FR_OK) {
        printf("无法打开目录: %s，错误码: %d\r\n", path, res);
        return;
    }

    while (1) 
	{
        res = f_readdir(&dir, &fileinfo);
        if (res != FR_OK || fileinfo.fname[0] == 0) break;

        if (fileinfo.fname[0] == '.') continue;

        if (fileinfo.fattrib & AM_DIR) {
            printf("[目录] %s/\r\n", fileinfo.fname);
        } else {
            printf("[文件] %s (大小: %lu 字节)\r\n", fileinfo.fname, (unsigned int)fileinfo.fsize);
        }
    }

    f_closedir(&dir);
}
