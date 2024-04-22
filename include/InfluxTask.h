#ifndef INFLUXTASK_H
#define INFLUXTASK_H
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "config.h"

class InfluxTask
{
    private:
        Point influxSensor;
        InfluxDBClient influxClient;
    public:
        InfluxTask();
        bool connectIfNotConnected();
        bool sendSensorData(float temp, float hum, unsigned int ldr);
        String getLastErrorMessage();
};

#endif