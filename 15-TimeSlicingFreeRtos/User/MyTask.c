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
                (UBaseType_t)2,
                (TaskHandle_t *)&task2_handle);

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    uint16_t count = 0;
    while (1)
    {
        taskENTER_CRITICAL();
        printf("task1:[%d]\r\n", ++count);
        Delay_ms(10);
        taskEXIT_CRITICAL();
    }
}

void task2(void *pvParameters)
{
    uint16_t count = 0;
    while (1)
    {
        taskENTER_CRITICAL();
        printf("task2:[%d]\r\n", ++count);
        Delay_ms(10);
        taskEXIT_CRITICAL();
    }
}
