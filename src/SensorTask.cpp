#include "SensorTask.h"
#include "config.h"

SensorTask::SensorTask() : dht(pinDHT, DHT11)
{
    lastTempReading = 0;
    lastHumReading = 0;
    lastReadingTime = 0;
    dht.begin();
}

void SensorTask::runTask()
{
    lastTempReading = dht.readTemperature();
    lastHumReading = dht.readHumidity();
    lastLdrReading = analogRead(pinLDR);
    lastReadingTime = millis();
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