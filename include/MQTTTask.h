#ifndef MQTTTASK_H
#define MQTTTASK_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <WiFi.h>
#include "config.h"
#include "SensorTask.h"

enum AlarmState
{
    ALARM_TRIGGERED,
    ALARM_RESET
};

class MQTTTask
{
private:
    esp_mqtt_client_config_t mqttClientConfig;;
    esp_mqtt_client_handle_t mqttClientHandle;
    QueueHandle_t SensorAlarmQueue;
    bool publishMessage(String topic, String payload);
public:
    MQTTTask();
    void start(QueueHandle_t sensorAlarmQueue);
    bool connect();
    bool sendLightAlarm(int lightValue, AlarmState state);
    QueueHandle_t getSensorAlarmQueue();
};

#endif