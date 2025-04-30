#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "LEDandKey.h"
#include "Serial.h"
#include "Delay.h"

#define START_TASK_STACK 128
void start_task(void *pvParameters);
TaskHandle_t start_task_handle;

#define TASK1_STACK 128
void task1(void *pvParameters);
TaskHandle_t task1_handle;

void timer1_callback(TimerHandle_t xTimer);
void timer2_callback(TimerHandle_t xTimer);
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;

void freertos_start(void)
{
    timer1_handle = xTimerCreate(
        "timer1",
        (TickType_t)500,
        (BaseType_t)pdFALSE,
        (void *)1,
        (TimerCallbackFunction_t)timer1_callback);

    if (timer1_handle != NULL)
    {
        printf("软件定时器1创建成功....\r\n");
    }

    timer2_handle = xTimerCreate(
        "timer2",
        (TickType_t)1000,
        (BaseType_t)pdTRUE,
        (void *)2,
        (TimerCallbackFunction_t)timer2_callback);

    if (timer2_handle != NULL)
    {
        printf("软件定时器2创建成功....\r\n");
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

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    uint8_t key;
    BaseType_t res;
    while (1)
    {
        if (GetKey())
        {
            key %= 2;
            key += 1;
            if (key == 1)
            {
                res = xTimerStart(timer1_handle, portMAX_DELAY);
                if (res == pdPASS)
                {
                    printf("timer1 start!\r\n");
                }
                res = xTimerStart(timer2_handle, portMAX_DELAY);
                if (res == pdPASS)
                {
                    printf("timer2 start!\r\n");
                }
            }
            else if (key == 2)
            {
                res = xTimerStop(timer1_handle, portMAX_DELAY);
                if (res == pdPASS)
                {
                    printf("timer1 stop!\r\n");
                }
                res = xTimerStop(timer2_handle, portMAX_DELAY);
                if (res == pdPASS)
                {
                    printf("timer2 stop!\r\n");
                }
            }
        }
    }
}

void timer1_callback(TimerHandle_t xTimer)
{
    static uint8_t count = 0;
    printf("timer1回调 %d 次\r\n", ++count);
}
void timer2_callback(TimerHandle_t xTimer)
{
    static uint8_t count = 0;
    printf("timer2回调 %d 次\r\n", ++count);
}
