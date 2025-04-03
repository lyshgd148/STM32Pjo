#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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

QueueHandle_t queue1, big_queue;

void freertos_start(void)
{
    queue1 = xQueueCreate(2, sizeof(uint8_t));

    if (queue1 != NULL)
    {
        printf("创建queue1,Successful!\r\n");
    }
    else
    {
        printf("创建queue1,Failed!\r\n");
    }
    big_queue = xQueueCreate(1, sizeof(char *));
    if (queue1 != NULL)
    {
        printf("创建big_queue,Successful!\r\n");
    }
    else
    {
        printf("创建big_queue,Failed!\r\n");
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
    uint8_t num=0;
    BaseType_t state;
    char *p = "dasdasdasdqwecasdca213124#$@#@#^&*";
    while (1)
    {
        if (GetKey())
        {
            num %= 3;
            num += 1;
            if (num == 1 || num == 2)
            {
                state = xQueueSend(queue1, &num, portMAX_DELAY);
                if (state == pdPASS)
                {
                    printf("向queue1 发送数据 [%d] 成功\r\n", num);
                }
            }
            else if (num == 3)
            {
                state = xQueueSend(big_queue, &p, portMAX_DELAY);
                if (state == pdPASS)
                {
                    printf("向big_queue 发送数据成功!\r\n");
                }
            }
        }
        vTaskDelay(100);
    }
}

void task2(void *pvParameters)
{
    BaseType_t state;
    uint8_t receive;
    while (1)
    {
        state = xQueueReceive(queue1, &receive, portMAX_DELAY);
        if (state == pdPASS)
        {
            printf("queue1 接收数据 [%d] Successful!\r\n", receive);
        }
    }
}

void task3(void *pvParameters)
{

    BaseType_t state;
    char *receive;
    while (1)
    {
        state = xQueueReceive(big_queue, &receive, portMAX_DELAY);
        if (state == pdPASS)
        {
            printf("big_queue 接收数据 [ %s ] Successful!\r\n", receive);
        }
    }
}
