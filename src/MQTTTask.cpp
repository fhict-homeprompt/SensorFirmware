#include "MQTTTask.h"

MQTTTask::MQTTTask()
{
    mqttClientConfig.host = mqttBroker;
    mqttClientConfig.port = mqttPort;
    mqttClientHandle = esp_mqtt_client_init(&mqttClientConfig);
}

static void runTask(void *pvParameters)
{
    MQTTTask *task = (MQTTTask *)pvParameters;
    SensorAlarmMessage message;

    while (true)
    {
        if (xQueueReceive(task->getSensorAlarmQueue(), &message, 0) == pdTRUE)
        {
            if (message.type == LDR_TRESHOLD_EXCEEDED)
            {
                task->sendLightAlarm(message.ldrReading);
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
    return (esp_mqtt_client_start(mqttClientHandle) == ESP_OK);
}

bool MQTTTask::publishMessage(String topic, String payload)
{
    String fullTopic = mqttTopic + topic;
    return (esp_mqtt_client_publish(mqttClientHandle, fullTopic.c_str(), payload.c_str(), payload.length(), 0, 0) != -1);
}

bool MQTTTask::sendLightAlarm(int lightValue)
{
    String payload = "LightTresholdExceeded," +
                     String(lightValue);
    return publishMessage("alert", payload);
}

QueueHandle_t MQTTTask::getSensorAlarmQueue()
{
    return SensorAlarmQueue;
}