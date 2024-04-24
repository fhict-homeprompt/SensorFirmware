#include "MQTTTask.h"

MQTTTask::MQTTTask(AppConfig::MqttConfig config)
{
    mqttConfig = config;
    mqttClientConfig = {};
    mqttClientConfig.host = mqttConfig.mqttBroker;
    mqttClientConfig.port = mqttConfig.mqttPort;
    mqttClientHandle = esp_mqtt_client_init(&mqttClientConfig);
    connected = false;
}

static void runTask(void *pvParameters)
{
    MQTTTask *task = (MQTTTask *)pvParameters;
    SensorAlarmMessage message;

    while (true)
    {
        if (xQueueReceive(task->getSensorAlarmQueue(), &message, 0) == pdTRUE)
        {
            if (!task->isConnected())
            {
                Serial.println("mqtt: Connecting to Broker");
                if (!task->connect())
                {
                    Serial.println("mqtt: No connection to MQTT");
                    Serial.println("mqtt: Discarded alarm");
                    continue;
                }
                else
                {
                    Serial.println("mqtt: Connected to Broker");
                }
            }
            switch (message.type)
            {
                case LDR_TRESHOLD_EXCEEDED:
                    task->sendLightAlarm(message.ldrReading, ALARM_TRIGGERED);
                    break;
                case LDR_TRESHOLD_NORMAL:
                    task->sendLightAlarm(message.ldrReading, ALARM_RESET);
                    break;
            }
        }
        vTaskDelay(100);
    }
}

void MQTTTask::start(QueueHandle_t sensorAlarmQueue)
{
    this->SensorAlarmQueue = sensorAlarmQueue;
    xTaskCreate(runTask, "mqttTask", 2048, this, taskMediumPriority, NULL);
}

bool MQTTTask::connect()
{
    connected = (esp_mqtt_client_start(mqttClientHandle) == ESP_OK);
    return connected;
}

bool MQTTTask::publishMessage(String topic, String payload)
{
    String fullTopic = String(mqttConfig.mqttTopic) + "/" + topic;
    return (esp_mqtt_client_publish(mqttClientHandle, fullTopic.c_str(), payload.c_str(), payload.length(), 0, 0) != -1);
}

bool MQTTTask::sendLightAlarm(int lightValue, AlarmState state)
{
    String eventName = state == ALARM_TRIGGERED ? "LightTresholdExceeded" : "LightTresholdNormal";
    String payload = eventName + "," + String(lightValue);
    if (!publishMessage("alert", payload))
    {
        Serial.println("mqtt: Failed to publish message");
        connected = false;
        esp_mqtt_client_disconnect(mqttClientHandle);
        return false;
    }
    return true;
}

QueueHandle_t MQTTTask::getSensorAlarmQueue()
{
    return SensorAlarmQueue;
}

bool MQTTTask::isConnected()
{
    return connected;
}