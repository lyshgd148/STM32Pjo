#include "FreeRTOS.h"
#include "task.h"
#include "LEDandKey.h"

#define START_TASK_STACK 128
void start_task(void *pvParameters);
StackType_t start_task_statck[START_TASK_STACK];
StaticTask_t start_task_tcb;
TaskHandle_t start_task_handle;

#define TASK1_STACK 128
void task1(void *pvParameters);
StackType_t task1_statck[START_TASK_STACK];
StaticTask_t task1_tcb;
TaskHandle_t task1_handle;

#define TASK2_STACK 128
void task2(void *pvParameters);
StackType_t task2_statck[START_TASK_STACK];
StaticTask_t task2_tcb;
TaskHandle_t task2_handle;

#define TASK3_STACK 128
void task3(void *pvParameters);
StackType_t task3_statck[START_TASK_STACK];
StaticTask_t task3_tcb;
TaskHandle_t task3_handle;

StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
StaticTask_t idle_task_tcb;

StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];
StaticTask_t timer_task_tcb;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                              StackType_t **ppxIdleTaskStackBuffer,
                              uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                               StackType_t **ppxTimerTaskStackBuffer,
                               uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer=&timer_task_tcb;
    *ppxTimerTaskStackBuffer=timer_task_stack;
    *pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}

void freertos_start(void)
{
    start_task_handle = xTaskCreateStatic((TaskFunction_t)start_task,
                                          (char *)"start_task",
                                          (uint32_t)START_TASK_STACK,
                                          (void *)NULL,
                                          (UBaseType_t)1,
                                          (StackType_t *)start_task_statck,
                                          (StaticTask_t *)&start_task_tcb);

    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    task1_handle = xTaskCreateStatic((TaskFunction_t)task1,
                                     (char *)"task1",
                                     (uint32_t)TASK1_STACK,
                                     (void *)NULL,
                                     (UBaseType_t)1,
                                     (StackType_t *)task1_statck,
                                     (StaticTask_t *)&task1_tcb);

    task2_handle = xTaskCreateStatic((TaskFunction_t)task2,
                                     (char *)"task2",
                                     (uint32_t)TASK2_STACK,
                                     (void *)NULL,
                                     (UBaseType_t)1,
                                     (StackType_t *)task2_statck,
                                     (StaticTask_t *)&task2_tcb);

    task3_handle = xTaskCreateStatic((TaskFunction_t)task3,
                                     (char *)"task3",
                                     (uint32_t)TASK3_STACK,
                                     (void *)NULL,
                                     (UBaseType_t)1,
                                     (StackType_t *)task3_statck,
                                     (StaticTask_t *)&task3_tcb);

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
        if (GetKey() == 1)
        {
            if (task1_handle != NULL)
            {
                vTaskDelete(task1_handle);
                task1_handle = NULL;
            }
        }
        vTaskDelay(500);
    }
}
