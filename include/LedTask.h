#ifndef LEDTASK_H
#define LEDTASK_H

#include <Arduino.h>
#include "config.h"

extern QueueHandle_t LedQueue;

enum BoardStatus
{
    BOARD_STATUS_OK,
    BOARD_STATUS_INITIALIZING,
    BOARD_STATUS_ALARM,
    BOARD_STATUS_ERROR
};

class LedTask
{
    public:
        LedTask();
        void start();
        static void setLedStatus(BoardStatus status);
};

#endif