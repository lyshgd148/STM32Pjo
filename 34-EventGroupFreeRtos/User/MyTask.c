#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

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

EventGroupHandle_t event_group;

void freertos_start(void)
{
    event_group = xEventGroupCreate();
    if (event_group != NULL)
    {
        printf("事件组创建成功!\r\n");
    }

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

#define Event_Bit0 (1 << 0)
#define Event_Bit1 (1 << 1)

void task1(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        if (GetKey())
        {
            key %= 2;
            key += 1;
            if (key == 1)
            {
                xEventGroupSetBits(event_group, Event_Bit0);
                printf("Bit1置位!\r\n");
            }
            else if (key == 2)
            {
                xEventGroupSetBits(event_group, Event_Bit1);
                printf("Bit2置位!\r\n");
            }
        }
    }
}

void task2(void *pvParameters)
{
    EventBits_t bits;
    while (1)
    {
        bits = xEventGroupWaitBits(event_group, Event_Bit0 | Event_Bit1, pdTRUE, pdTRUE, portMAX_DELAY);
        printf("接收到的事件标志组=%#x \r\n", bits);
    }
}
