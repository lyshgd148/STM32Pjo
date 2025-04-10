#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Serial.h"
#include "MySD.h"

uint8_t Data[1024];
uint8_t cid[16];
uint8_t csd[16];
int main(void)
{

    
    uint16_t i, num, state = 0;

    Serial_Init();
    state = SD_Init();
    num = 1;


    printf("扇区数量:%d\r\n", SD_Get_SectorNum());
    printf("SD卡大小:%fG\r\n", SD_Get_SectorNum()*0.5/1024/1024);
    SD_Get_cid(cid);

    printf("CID:");
    for(i=0;i<16;i++)
    {
        printf("%02X ", cid[i]);
    }
    printf("\r\n");

    printf("............................................\r\n");

    SD_Get_csd(csd);
    printf("CSD: ");
    for(i=0;i<16;i++)
    {
        printf("%02X ",csd[i]);
    }
    printf("\r\n");



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
