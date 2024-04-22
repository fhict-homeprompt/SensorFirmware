#include "LedTask.h"

const static char TASKNAME[] = "ledTask";

LedTask::LedTask()
{
    pinMode(14, OUTPUT);
    ledTaskHandle = NULL;
}

static void blinkLed(int delay, int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(14, HIGH);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        digitalWrite(14, LOW);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}

static void ledThread(void *pvParameters)
{
    while (true)
    {
        LedTask *task = (LedTask *)pvParameters;
        switch (task->boardStatus)
        {
        case BOARD_STATUS_OK:
            digitalWrite(14, HIGH);
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
    xTaskCreate(ledThread, TASKNAME, 1024, this, 1, NULL);
}

void LedTask::setLedStatus(BoardStatus status)
{
    boardStatus = status;
}