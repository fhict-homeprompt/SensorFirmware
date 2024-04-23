#include <Arduino.h>
#include "WiFi.h"
#include "SensorTask.h"
#include "MQTTTask.h"
#include "LedTask.h"
#include "config.h"

SensorTask sensorTask;
MQTTTask mqttTask;
LedTask ledTask;

QueueHandle_t SensorAlarmQueue;
QueueHandle_t LedQueue;

SensorAlarm sensorAlarm({.queue = NULL,
                         .ldrTreshold = ldrTreshold,
                         .ldrTriggerDelay = ldrTriggerDelay,
                         .ldrResetDelay = ldrResetDelay});

void initializePeripherals()
{
  Serial.begin(9600);
  ledTask.start();
  ledTask.setLedStatus(BOARD_STATUS_INITIALIZING);
}

void initializeAndWaitForWLAN()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(wlanName, wlanPassword);
  Serial.println(WiFi.macAddress());
  Serial.println("\nConnecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void initializeServices()
{
  Serial.println("Connecting to MQTT Broker");
  if (!mqttTask.connect())
  {
    Serial.println("Failed to connect to MQTT broker");
  }
  else
  {
    Serial.println("Connected to MQTT broker");
  }
  mqttTask.start(SensorAlarmQueue);
  sensorTask.setAlarm(&sensorAlarm);
  sensorTask.start();
  ledTask.setLedStatus(BOARD_STATUS_OK);
}

void initializeQueues()
{
  SensorAlarmQueue = xQueueCreate(5, sizeof(SensorAlarmMessage));
  LedQueue = xQueueCreate(5, sizeof(BoardStatus));
  sensorAlarm.setQueue(SensorAlarmQueue);
}

void setup()
{
  vTaskPrioritySet(NULL, taskMediumPriority);
  initializeQueues();
  initializePeripherals();
  initializeAndWaitForWLAN();
  initializeServices();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(wlanName, wlanPassword);
    while (WiFi.status() != WL_CONNECTED)
    {
      vTaskDelay(100);
    }
    Serial.println("WiFi reconnected");
  }
}