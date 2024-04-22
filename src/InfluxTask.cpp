#include "InfluxTask.h"

InfluxTask::InfluxTask() : influxSensor(influxDBSensor),
                           influxClient(influxDBHost,
                                        influxDBOrg,
                                        influxDBBucket,
                                        influxDBToken,
                                        InfluxDbCloud2CACert)
{
}

bool InfluxTask::connectIfNotConnected()
{
    return influxClient.validateConnection();
}

bool InfluxTask::sendSensorData(float temp, float hum, unsigned int ldr)
{
    influxSensor.clearFields();
    influxSensor.addField("temperature", temp);
    influxSensor.addField("humidity", hum);
    influxSensor.addField("lightlevel", ldr);
    return influxClient.writePoint(influxSensor);
}

String InfluxTask::getLastErrorMessage()
{
    return influxClient.getLastErrorMessage();
}