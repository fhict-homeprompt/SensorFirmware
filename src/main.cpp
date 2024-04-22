#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "WiFi.h"
#include "SensorTask.h"
#include "InfluxTask.h"
#include "MQTTTask.h"
#include "LedTask.h"
#include "config.h"

SensorTask sensorTask;
MQTTTask mqttTask;
LedTask ledTask;

#ifdef ENABLE_INFLUXDB
InfluxTask influxTask;
#endif

void setup()
{
  Serial.begin(9600);
  Serial.println(WiFi.macAddress());
  ledTask.start();
  ledTask.setLedStatus(BOARD_STATUS_INITIALIZING);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wlanName, wlanPassword);
  Serial.println("\nConnecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Syncing time");
  timeSync(timezone, "pool.ntp.org", "time.nis.gov");
#ifdef ENABLE_INFLUXDB
  Serial.println("Connecting to InfluxDB");
  if (!influxTask.connectIfNotConnected())
  {
    Serial.println("Failed to connect to InfluxDB");
    Serial.println(influxTask.getLastErrorMessage());
  }
#endif
  Serial.println("Connecting to MQTT Broker");
  if (!mqttTask.connect())
  {
    Serial.println("Failed to connect to MQTT broker");
  } else {
    Serial.println("Connected to MQTT broker");
  }
  ledTask.setLedStatus(BOARD_STATUS_OK);
}

void loop()
{
  sensorTask.runTask();
  Serial.printf("Read time: %u\n\tLDR Sensor: %d/4095\n\tDHT Temp: %f C\n\tDHT Humidity: %f\n",
                sensorTask.getLastReadingTime(),
                sensorTask.getLdrReading(),
                sensorTask.getTempReading(),
                sensorTask.getHumReading());
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(wlanName, wlanPassword);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(100);
    }
    Serial.println("WiFi reconnected");
  }
#ifdef ENABLE_INFLUXDB
  if ((!influxTask.connectIfNotConnected()) ||
      (!influxTask.sendSensorData(sensorTask.getTempReading(), sensorTask.getHumReading(), sensorTask.getLdrReading())))
  {
    Serial.println("Failed to send data to InfluxDB");
    Serial.println(influxTask.getLastErrorMessage());
  }
#endif
  vTaskDelay(refreshTime / portTICK_PERIOD_MS);
}