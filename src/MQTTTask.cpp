#include "MQTTTask.h"

MQTTTask::MQTTTask()
{
    mqttClientConfig.host = mqttBroker;
    mqttClientConfig.port = mqttPort;
    mqttClientHandle = esp_mqtt_client_init(&mqttClientConfig);
}

bool MQTTTask::connect()
{
    return (esp_mqtt_client_start(mqttClientHandle) == ESP_OK);
}

bool MQTTTask::publishMessage(String topic, String payload)
{
    String fullTopic = mqttTopic + topic;
    return (esp_mqtt_client_publish(mqttClientHandle, fullTopic.c_str(), payload.c_str(), payload.length(), 0, 0) != -1);
}

bool MQTTTask::sendLightAlarm(int lightValue, int duration)
{
    String payload = "LampOn," +
                     String(lightValue) +
                     "," +
                     String(duration);
    return publishMessage("alert", payload);
}