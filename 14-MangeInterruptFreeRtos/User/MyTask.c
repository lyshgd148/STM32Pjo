#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h"
#include "LEDandKey.h"
#include "Serial.h"

extern uint8_t i;

#define START_TASK_STACK 128
void start_task(void *pvParameters);
TaskHandle_t start_task_handle;

#define TASK1_STACK 128
void task1(void *pvParameters);
TaskHandle_t task1_handle;

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

    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    printf("开启task1...\r\n");
    uint8_t Key_state = 0;
    while (1)
    {

        if (GetKey()==1)
        {
            Key_state %= 2;
            Key_state += 1;
            if (Key_state == 1)
            {
                portDISABLE_INTERRUPTS();
                printf("K1!\r\n");
            }
            else
            {
                portENABLE_INTERRUPTS();
                printf("K2!\r\n");
            }
        }
        LED1_ON();
        Delay_ms(500);
        LED1_OFF();
        Delay_ms(500);
    }
}
