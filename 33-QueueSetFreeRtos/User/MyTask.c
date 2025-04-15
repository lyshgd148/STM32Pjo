#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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

QueueHandle_t queue1, sem_handle;
QueueSetHandle_t queue_set_handle;

void freertos_start(void)
{
    BaseType_t res;

    queue1 = xQueueCreate(2, sizeof(uint8_t));

    if (queue1 != NULL)
    {
        printf("创建queue1,Successful!\r\n");
    }

    sem_handle=xSemaphoreCreateBinary();
    if (sem_handle != NULL)
    {
        printf("创建二值信号量,Successful!\r\n");
    }

    queue_set_handle = xQueueCreateSet(2);
    if (queue_set_handle != NULL)
    {
        printf("创建队列集成功\r\n");
    }

    res = xQueueAddToSet(queue1, queue_set_handle);
    if (res == pdPASS)
    {
        printf("queue1添加成功\r\n");
    }
    res = xQueueAddToSet(sem_handle, queue_set_handle);
    if (res == pdPASS)
    {
        printf("semphaore添加成功\r\n");
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
    uint8_t num = 0;
    BaseType_t state;
    while (1)
    {
        if (GetKey())
        {
            num %= 2;
            num += 1;
            if (num == 1)
            {
                state = xQueueSend(queue1, &num, portMAX_DELAY);
                if (state == pdPASS)
                {
                    printf("向queue1 发送数据 [%d] 成功\r\n", num);
                }
            }
            else if (num == 2)
            {
                state = xSemaphoreGive(sem_handle);
                if (state == pdPASS)
                {
                    printf("释放二值信号成功!\r\n");
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
    QueueSetMemberHandle_t mem;
    while (1)
    {
        mem = xQueueSelectFromSet(queue_set_handle, portMAX_DELAY);

        if (mem == queue1)
        {
            state = xQueueReceive(queue1, &receive, portMAX_DELAY);
            if (state == pdPASS)
            {
                printf("queue1 接收数据 [%d] Successful!\r\n", receive);
            }
        }
        else if (mem == sem_handle)
        {
            state = xSemaphoreTake(sem_handle, portMAX_DELAY);
            if (state == pdPASS)
            {
                printf("获取二值信号量成功\r\n");
            }
        }
    }
}
