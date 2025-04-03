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

#define TASK2_STACK 516
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
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();

    while (1)
    {
        LED1_ON();
        vTaskDelayUntil(&pxPreviousWakeTime, 500);
        LED1_OFF();
        vTaskDelayUntil(&pxPreviousWakeTime, 500);
    }
}



void task2(void *pvParameters)
{
    char str[200];
    UBaseType_t status, num,freeStock;
    TaskStatus_t sys_task_status[10];
    TaskStatus_t s_task_status;
    TaskHandle_t task_handle;
    eTaskState now_status;

    status = uxTaskPriorityGet(task1_handle);
    printf("任务1的priority:%d\r\n", status);

    vTaskPrioritySet(task1_handle, 4);
    status = uxTaskPriorityGet(task1_handle);
    printf("任务1的priority:%d\r\n", status);

    num = uxTaskGetNumberOfTasks();
    printf("系统中任务的数量%d\r\n", num);

    uxTaskGetSystemState(sys_task_status,
                         num,
                         NULL);
    printf("任务名\t\t\t任务编号\t\t\t任务状态\t\t\t任务优先级\r\n");
    for (uint8_t i = 0; i < num; i++)
    {
        printf("%s\t\t\t%d\t\t\t\t%d\t\t\t%d\r\n",
               sys_task_status[i].pcTaskName,
               sys_task_status[i].xTaskNumber,
               sys_task_status[i].eCurrentState,
               sys_task_status[i].uxCurrentPriority);
    }

    vTaskGetInfo(task1_handle,&s_task_status,pdTRUE,eInvalid);
    printf("任务名称:%s\r\n",s_task_status.pcTaskName);
    printf("任务编号:%d\r\n",s_task_status.xTaskNumber);
    printf("任务优先级:%d\r\n",s_task_status.uxCurrentPriority);
    printf("任务状态:%d\r\n",s_task_status.eCurrentState);

    task_handle=xTaskGetCurrentTaskHandle();
    printf("task_handle句柄%p,当前任务句柄%p\r\n",task_handle,task2_handle);

    task_handle=xTaskGetHandle("task1");
    printf("任务1句柄%p\r\n",task_handle);
    
    freeStock=uxTaskGetStackHighWaterMark(task2_handle);
    printf("任务2的历史剩余最小值%d\r\n",freeStock);

    now_status=eTaskGetState(task2_handle);
    printf("任务2的状态%d\r\n",now_status);

    vTaskList(str);
    printf("%s",str);

    while (1)
    {
        vTaskDelay(500);
    }
}
