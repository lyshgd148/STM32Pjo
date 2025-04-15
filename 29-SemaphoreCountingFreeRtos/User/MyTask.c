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

QueueHandle_t semaphore_handle;

void freertos_start(void)
{
    UBaseType_t count;

    semaphore_handle = xSemaphoreCreateCounting(20, 0);
    if (semaphore_handle != NULL)
    {
        count = uxSemaphoreGetCount(semaphore_handle);
        printf("创建计数计数信号量成功,初始计数%d\r\n", count);
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

void task1(void *pvParameters)
{
    BaseType_t res;
    
    while (1)
    {
        if (GetKey())
        {
            res = xSemaphoreGive(semaphore_handle);
            if (res == pdPASS)
            {
                printf("Task1释放信号量成功!\r\n");
            }
            {
                printf("Task1释放信号量失败!\r\n");
            }
        }
        vTaskDelay(500);
    }
}

void task2(void *pvParameters)
{
    BaseType_t res;
    UBaseType_t count;
    while (1)
    {
        res = xSemaphoreTake(semaphore_handle, NULL);
        if(res==pdPASS)
        {
            printf("Task2获取信号量成功!\r\n");
        }
        else
        {
            printf("Task2获取信号量失败!\r\n");
        }
        count = uxSemaphoreGetCount(semaphore_handle);
        printf("当前计数%d\r\n", count);
        vTaskDelay(1000);
    }
}
