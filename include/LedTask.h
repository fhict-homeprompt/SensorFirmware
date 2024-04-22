#ifndef LEDTASK_H
#define LEDTASK_H

#include <Arduino.h>

enum BoardStatus
{
    BOARD_STATUS_OK,
    BOARD_STATUS_INITIALIZING,
    BOARD_STATUS_ALARM,
    BOARD_STATUS_ERROR
};

class LedTask
{
    private:
        TaskHandle_t ledTaskHandle;
    public:
        BoardStatus boardStatus;
        LedTask();
        void start();
        void setLedStatus(BoardStatus status);
};

#endif