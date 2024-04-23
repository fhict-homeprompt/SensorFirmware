#include "SensorTask.h"
#include "config.h"

const static char TASKNAME[] = "sensorTask";

SensorTask::SensorTask() : dht(pinDHT, DHT11)
{
    lastTempReading = 0;
    lastHumReading = 0;
    lastReadingTime = 0;
    alarm = NULL;
    dht.begin();
}

static void runTask(void *pvParameters)
{
    SensorTask *task = (SensorTask *)pvParameters;
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (true)
    {
        task->readSensors();
        xTaskDelayUntil(&lastWakeTime, refreshTime / portTICK_PERIOD_MS);
    }
}

void SensorTask::start()
{
    xTaskCreate(runTask, TASKNAME, 2048, this, taskHighPriority, NULL);
}

void SensorTask::readSensors()
{
    lastTempReading = dht.readTemperature();
    lastHumReading = dht.readHumidity();
    lastLdrReading = analogRead(pinLDR);
    lastReadingTime = millis();
    if (alarm != NULL)
    {
        alarm->checkAlarm(lastLdrReading);
    }
}

float SensorTask::getTempReading()
{
    return lastTempReading;
}

float SensorTask::getHumReading()
{
    return lastHumReading;
}

unsigned int SensorTask::getLdrReading()
{
    return lastLdrReading;
}

unsigned long SensorTask::getLastReadingTime()
{
    return lastReadingTime;
}

void SensorTask::setAlarm(SensorAlarm *alarm)
{
    this->alarm = alarm;
}

SensorAlarm::SensorAlarm(SensorAlarmConfig config)
{
    this->config = config;
    ldrTresholdLastTriggered = 0;
    ldrTresholdExceeded = false;
}

void SensorAlarm::checkAlarm(unsigned int ldrReading)
{
    unsigned long currentTime = millis();

    if (ldrReading > config.ldrTreshold)
    {
        if (ldrTresholdLastTriggered == 0)
        {
            ldrTresholdLastTriggered = currentTime;
        }
        else if (!ldrTresholdExceeded)
        {
            if (currentTime - ldrTresholdLastTriggered > config.ldrTriggerDelay)
            {
                ldrTresholdExceeded = true;
                ldrTresholdLastTriggered = millis();
                sendLdrAlarmMessage(ldrReading);
            }
        }
    }
    else if (ldrTresholdExceeded)
    {
        if (currentTime - ldrTresholdLastTriggered > config.ldrResetDelay)
        {
            ldrTresholdExceeded = false;
            ldrTresholdLastTriggered = 0;
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