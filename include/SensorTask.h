#ifndef SENSORTASK_H
#define SENSORTASK_H

#include <Arduino.h>
#include <DHT.h>

struct SensorAlarmConfig
{
    QueueHandle_t queue;
    unsigned int ldrTreshold;
    unsigned long ldrTriggerDelay;
    unsigned long ldrResetDelay;
};

enum SensorAlarmMessageType
{
    LDR_TRESHOLD_EXCEEDED,
    LDR_TRESHOLD_NORMAL
};

struct SensorAlarmMessage
{
    SensorAlarmMessageType type;
    unsigned long timestamp;
    unsigned int ldrReading;
};

class SensorAlarm
{
private:
    SensorAlarmConfig config;
    bool ldrTresholdExceeded;
    unsigned long ldrTresholdLastTriggered;

    void sendLdrAlarmMessage(unsigned int ldrReading);

public:
    SensorAlarm(SensorAlarmConfig config);
    void checkAlarm(unsigned int ldrReading);
    void setQueue(QueueHandle_t queue);
};

class SensorTask
{
private:
    DHT dht;
    SensorAlarm *alarm;
    float lastTempReading;
    float lastHumReading;
    unsigned int lastLdrReading;
    unsigned long lastReadingTime;

public:
    SensorTask();
    void readSensors();
    void setAlarm(SensorAlarm *alarm);
    void start();
    float getTempReading();
    float getHumReading();
    unsigned int getLdrReading();
    unsigned long getLastReadingTime();
};

#endif