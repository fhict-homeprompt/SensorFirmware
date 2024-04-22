#ifndef MQTTTASK_H
#define MQTTTASK_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <WiFi.h>
#include "config.h"

class MQTTTask
{
private:
    esp_mqtt_client_config_t mqttClientConfig;;
    esp_mqtt_client_handle_t mqttClientHandle;
    bool publishMessage(String topic, String payload);
public:
    MQTTTask();
    bool connect();
    bool sendLightAlarm(int lightValue, int duration);
};

#endif