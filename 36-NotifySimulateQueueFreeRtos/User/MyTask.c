#include "FreeRTOS.h"
#include "task.h"
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
    BaseType_t res;
    uint8_t key = 0;
    while (1)
    {
        if (GetKey())
        {
            key %= 2;
            key += 1;
            res = xTaskNotify(task2_handle, key, eSetValueWithOverwrite);
            if (res == pdPASS)
            {
                printf("task1发送任务通知成功!\r\n");
            }
        }

        vTaskDelay(500);
    }
}

void task2(void *pvParameters)
{
    uint32_t value;
    BaseType_t res;
    while (1)
    {

        res = xTaskNotifyWait(
            0x00000000,
            0xffffffff,
            &value,
            portMAX_DELAY);
        if (res == pdPASS)
        {
            printf("接受到的通知值%d\r\n", value);
        }
        
    }
}
