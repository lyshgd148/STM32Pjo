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

QueueHandle_t semaphore;
void freertos_start(void)
{
    vSemaphoreCreateBinary(semaphore);
    if (semaphore != NULL)
    {
        printf("二值信号量创建成功!\r\n");
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
    BaseType_t state;
    while (1)
    {
        if (GetKey())
        {
            state = xSemaphoreGive(semaphore);
            if (state == pdPASS)
            {
                printf("释放二值信号量成功!\r\n");
            }
        }
    }
}

void task2(void *pvParameters)
{
    BaseType_t state;
    while (1)
    {
        state = xSemaphoreTake(semaphore, portMAX_DELAY);
        if (state == pdPASS)
        {
            printf("获取二值信号量成功!\r\n");
        }
    }
}
