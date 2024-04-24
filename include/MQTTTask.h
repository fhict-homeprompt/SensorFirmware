#ifndef MQTTTASK_H
#define MQTTTASK_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <WiFi.h>
#include "Configuration.h"
#include "SensorTask.h"

enum AlarmState
{
    ALARM_TRIGGERED,
    ALARM_RESET
};

class MQTTTask
{
private:
    esp_mqtt_client_config_t mqttClientConfig;
    esp_mqtt_client_handle_t mqttClientHandle;
    QueueHandle_t SensorAlarmQueue;
    bool publishMessage(String topic, String payload);
    bool connected;

public:
    AppConfig::MqttConfig mqttConfig;
    MQTTTask(AppConfig::MqttConfig config);
    void start(QueueHandle_t sensorAlarmQueue);
    bool sendLightAlarm(int lightValue, AlarmState state);
    bool sendAliveMessage();
    bool connect();
    bool isConnected();
    QueueHandle_t getSensorAlarmQueue();
};

#endif