#include "LedTask.h"

const static char TASKNAME[] = "ledTask";

LedTask::LedTask()
{
    pinMode(pinLED, OUTPUT);
}

static void blinkLed(int delay, int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(pinLED, HIGH);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        digitalWrite(pinLED, LOW);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}

static void ledThread(void *pvParameters)
{
    BoardStatus boardStatus = BOARD_STATUS_OK;
    while (true)
    {
        xQueueReceive(LedQueue, &boardStatus, 0);
        switch (boardStatus)
        {
        case BOARD_STATUS_OK:
            digitalWrite(pinLED, HIGH);
            break;
        case BOARD_STATUS_INITIALIZING:
            blinkLed(750, 1);
            break;
        case BOARD_STATUS_ALARM:
            blinkLed(200, 3);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            break;
        case BOARD_STATUS_ERROR:
            blinkLed(250, 1);
            break;
        }
        vTaskDelay(0);
    }
}

void LedTask::start()
{
    xTaskCreate(ledThread, TASKNAME, 1024, NULL, taskLowPriority, NULL);
}

void LedTask::setLedStatus(BoardStatus status)
{
    xQueueSend(LedQueue, &status, 0);
}