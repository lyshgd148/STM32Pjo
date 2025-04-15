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

#define TASK3_STACK 128
void task3(void *pvParameters);
TaskHandle_t task3_handle;

QueueHandle_t semphr_handle;

void freertos_start(void)
{
    vSemaphoreCreateBinary(semphr_handle);
    if (semphr_handle != NULL)
    {
        printf("创建二值信号量成功!\r\n");
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
    xTaskCreate((TaskFunction_t)task3,
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
    BaseType_t res;
    while (1)
    {
        printf("Task1获取信号量!\r\n");
        res = xSemaphoreTake(semphr_handle, portMAX_DELAY);
        if (res != pdPASS)
        {
            printf("Task1获取信号量失败!\r\n");
        }
        printf("Task1在执行!\r\n");
        Delay_ms(3000);
        printf("Task1释放信号量...\r\n");
        res = xSemaphoreGive(semphr_handle);
        if (res != pdPASS)
        {
            printf("Task1释放信号量失败!\r\n");
        }
        vTaskDelay(1000);
    }
}

void task2(void *pvParameters)
{
    while (1)
    {
        printf("中优先级Task2在执行 \r\n");
       Delay_ms(1500);
        printf("中优先级Task2执行完成 \r\n");

        vTaskDelay(1000);
    }
}

void task3(void *pvParameters)
{
    BaseType_t res;
    while (1)
    {
        printf("Task3获取信号量!\r\n");
        res = xSemaphoreTake(semphr_handle, portMAX_DELAY);
        if (res != pdPASS)
        {
            printf("Task3获取信号量失败!\r\n");
        }
        printf("Task3在执行!\r\n");
        Delay_ms(1000);
        printf("Task3释放信号量...\r\n");
        res = xSemaphoreGive(semphr_handle);
        if (res != pdPASS)
        {
            printf("Task3释放信号量失败!\r\n");
        }
        vTaskDelay(1000);
    }
}
