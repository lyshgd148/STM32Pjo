#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Serial.h"
#include "MySD.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>

FATFS fs;        // 文件系统对象
FIL file;        // 文件对象
FRESULT res;     // 操作结果
UINT bw, br;     // 实际读写的字节数
char buffer[10240]; // 读写缓冲区


int main(void)
{
//    char *text = "Hello STM32 + FATFS!\r\n";
    Serial_Init();
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
    // 2. 创建并写入文件
//    res = f_open(&file, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if (res != FR_OK)
//    {
//        printf("打开/创建文件失败！错误码: %d\r\n", res);
//    }
//    
//    res = f_write(&file, text, strlen(text), &bw);
//    if (res == FR_OK && bw == strlen(text))
//    {
//        printf("文件写入成功\r\n");
//    }
//    else
//    {
//        printf("文件写入失败\r\n");
//    }
//    f_close(&file); // 关闭文件
    // 3. 打开并读取文件
    res = f_open(&file, "TEST.txt", FA_READ);
    if (res == FR_OK)
    {
        memset(buffer, 0, sizeof(buffer));
		
		while(f_eof(&file)==0)
		{
			res = f_read(&file, buffer, sizeof(buffer) - 1, &br);
			if (res == FR_OK)
			{
				printf("%s", buffer);
				memset(buffer, 0, sizeof(buffer));
			}
			else
			{
				printf("读取错误\r\n");
				break;
			}
		}
        f_close(&file);
    }
    else
    {
        printf("打开文件失败！\r\n");
    }

    // 4. 卸载文件系统
    f_mount(NULL, "", 1);

    while (1)
    {
        // SD_Read_Sector(num,2,Data);
        //     for (i=0;i<1024;i++)
        //     {
        //         printf("%d\r\n",Data[i]);
        //     }
        //     num+=2;
    }
}
