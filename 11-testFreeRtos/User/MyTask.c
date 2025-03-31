#include "FreeRTOS.h"
#include "task.h"
#include "LEDandKey.h"

#define START_TASK_STACK 128
void start_task(void *pvParameters);
TaskHandle_t start_task_handle;

#define TASK1_STACK 128
void task1(void *pvParameters);
TaskHandle_t task1_handle;

#define TASK2_STACK 128
void task2(void *pvParameters);
TaskHandle_t task2_handle;

#define TASK3_STACK 128
void task3(void *pvParameters);
TaskHandle_t task3_handle;

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

    xTaskCreate((TaskFunction_t)start_task,
                (char *)"task3",
                (configSTACK_DEPTH_TYPE)TASK3_STACK,
                (void *)NULL,
                (UBaseType_t)4,
                (TaskHandle_t *)&task3_handle);

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    while (1)
    {
        LED1_ON();
        vTaskDelay(500);
        LED1_OFF();
        vTaskDelay(500);
    }
}

void task2(void *pvParameters)
{
    while (1)
    {
        LED2_ON();
        vTaskDelay(500);
        LED2_OFF();
        vTaskDelay(500);
    }
}

void task3(void *pvParameters)
{
    while (1)
    {
        if (GetKey())
        {
            if (task1_handle != NULL)
            {
                vTaskDelete(task1_handle);
                task1_handle=NULL;
            }
        }
        vTaskDelay(500);
    }
}