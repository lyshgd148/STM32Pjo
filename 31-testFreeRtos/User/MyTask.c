#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "LEDandKey.h"
#include "Serial.h"
#include "Delay.h"

#define START_TASK_STACK 128
void start_task(void *pvParameters);
TaskHandle_t start_task_handle;

#define TASK1_STACK 128
void task1(void *pvParameters);
TaskHandle_t task1_handle;

#define TASK2_STACK 128
void task2(void *pvParameters);
TaskHandle_t task2_handle;



void freertos_start(void)
{
    xTaskCreate((TaskFunction_t)start_task,
                (char *)"start_task",
                (configSTACK_DEPTH_TYPE)START_TASK_STACK,
                (void *)NULL,
                (UBaseType_t)1,
                (TaskHandle_t *)&start_task_handle);

    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)task1,
                (char *)"task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK,
                (void *)NULL,
                (UBaseType_t)2,
                (TaskHandle_t *)&task1_handle);

    xTaskCreate((TaskFunction_t)task2,
                (char *)"task2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK,
                (void *)NULL,
                (UBaseType_t)3,
                (TaskHandle_t *)&task2_handle);

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    while (1)
    {
        printf("task1....1\r\n");
        Delay_ms(2500);          //当task1中Delay_ma()的时间大于task2的阻塞时间时,task2再调用Delay_ms()就会清除task1中Delay_ms的纪录。 (为什么会被清楚呢，应为我的定时器模式设置为1次性模式，哈哈哈我正是个小天才)
        printf("task1....2\r\n");
        vTaskDelay(1000);
    }
}

void task2(void *pvParameters)
{
    while (1)
    {
        printf("task2....\r\n");
        Delay_ms(1000);
        vTaskDelay(2000);
    }
}

