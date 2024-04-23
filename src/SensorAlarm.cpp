#include "SensorTask.h"

SensorAlarm::SensorAlarm(SensorAlarmConfig config)
{
    this->config = config;
    ldrTresholdLastTriggered = 0;
    ldrTresholdExceeded = false;
}

void SensorAlarm::checkAlarm(unsigned int ldrReading)
{
    unsigned long currentTime = millis();

    if (ldrReading > config.sensorConfig.ldrTreshold)
    {
        if (ldrTresholdLastTriggered == 0)
        {
            ldrTresholdLastTriggered = currentTime;
        }
        else if (!ldrTresholdExceeded)
        {
            if (currentTime - ldrTresholdLastTriggered > config.sensorConfig.ldrTriggerDelay)
            {
                ldrTresholdExceeded = true;
                ldrTresholdLastTriggered = millis();
                LedTask::setLedStatus(BOARD_STATUS_ALARM);
                sendLdrAlarmMessage(ldrReading);
            }
        }
    }
    else if (ldrTresholdExceeded)
    {
        if (currentTime - ldrTresholdLastTriggered > config.sensorConfig.ldrResetDelay)
        {
            ldrTresholdExceeded = false;
            ldrTresholdLastTriggered = 0;
            LedTask::setLedStatus(BOARD_STATUS_OK);
            sendLdrAlarmMessage(ldrReading);
        }
    }
}

void SensorAlarm::sendLdrAlarmMessage(unsigned int ldrReading)
{
    SensorAlarmMessage message;
    message.type = ldrTresholdExceeded ? LDR_TRESHOLD_EXCEEDED : LDR_TRESHOLD_NORMAL;
    message.ldrReading = ldrReading;
    xQueueSend(config.queue, &message, 0);
}

void SensorAlarm::setQueue(QueueHandle_t queue)
{
    config.queue = queue;
}