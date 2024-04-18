#ifndef SENSORTASK_H
#define SENSORTASK_H

#include <Arduino.h>
#include <DHT.h>

class SensorTask
{
private:
    DHT dht;
    float lastTempReading;
    float lastHumReading;
    unsigned int lastLdrReading;
    unsigned long lastReadingTime;

public:
    SensorTask();
    void runTask();
    float getTempReading();
    float getHumReading();
    unsigned int getLdrReading();
    unsigned long getLastReadingTime();
};

#endif