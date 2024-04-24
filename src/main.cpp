#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <SPIFFS.h>
#include "SensorTask.h"
#include "MQTTTask.h"
#include "LedTask.h"
#include "Configuration.h"

ConfigurationLoader configLoader;
LedTask ledTask;
MQTTTask *mqttTask;
SensorTask *sensorTask;
SensorAlarm *sensorAlarm;

AppConfig configuration;
QueueHandle_t SensorAlarmQueue;
QueueHandle_t LedQueue;

void initializePeripherals()
{
  pinMode(pinBTN, INPUT_PULLUP);
  ledTask.start();
  ledTask.setLedStatus(BOARD_STATUS_INITIALIZING);
}

void initializeAndWaitForWLAN()
{
  WiFiManager wifiManager;
  WiFi.mode(WIFI_STA);
  if (digitalRead(pinBTN) == LOW)
  {
    Serial.println("Resetting WiFi credentials");
    wifiManager.resetSettings();
  }
  if (wifiManager.autoConnect())
  {
    Serial.println("Connected to the WiFi network");
  }
  else
  {
    Serial.println("Failed to connect to the WiFi network");
    esp_restart();
  }
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void initializeServices()
{
  mqttTask = new MQTTTask(configuration.mqttConfig);
  sensorTask = new SensorTask();
  sensorAlarm = new SensorAlarm({.queue = SensorAlarmQueue,
                                 .sensorConfig = configuration.sensorConfig});

  mqttTask->start(SensorAlarmQueue);
  sensorTask->setAlarm(sensorAlarm);
  sensorTask->start();
  ledTask.setLedStatus(BOARD_STATUS_OK);
}

void initializeQueues()
{
  SensorAlarmQueue = xQueueCreate(5, sizeof(SensorAlarmMessage));
  LedQueue = xQueueCreate(5, sizeof(BoardStatus));
}

void initializeFilesystem()
{
  if (!SPIFFS.begin())
  {
    Serial.println("main: Failed to mount file system. Please reflash.");
    while (1) {}
  }
}

void setup()
{
  Serial.begin(9600);
  vTaskPrioritySet(NULL, taskMediumPriority);
  initializeFilesystem();
  if (!configLoader.loadConfig(&configuration))
  {
    Serial.println("main: Failed to load configuration. Using default configuration.");
    configLoader.loadDefaultConfig(&configuration);
  }
  initializeQueues();
  initializePeripherals();
  initializeAndWaitForWLAN();
  initializeServices();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("main: WiFi connection lost. Restarting ESP32!");
    esp_restart();
  }
  vTaskDelay(100);
}