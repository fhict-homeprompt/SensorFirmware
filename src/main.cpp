#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "WiFi.h"
#include "SensorTask.h"
#include "config.h"

SensorTask sensorTask;
Point influxSensor("livingroom_sensor");
InfluxDBClient influxClient(influxDBHost,
                            influxDBOrg,
                            influxDBBucket,
                            influxDBToken,
                            InfluxDbCloud2CACert);

void setup()
{
  Serial.begin(9600);
  Serial.println(WiFi.macAddress());
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
  Serial.println("Connecting to InfluxDB");
  if (influxClient.validateConnection())
  {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(influxClient.getServerUrl());
  }
  else
  {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
}

void loop()
{
  sensorTask.runTask();
  Serial.printf("Read time: %u\n\tLDR Sensor: %d/4095\n\tDHT Temp: %f C\n\tDHT Humidity: %f\n",
                sensorTask.getLastReadingTime(),
                sensorTask.getLdrReading(),
                sensorTask.getTempReading(),
                sensorTask.getHumReading());
  influxSensor.clearFields();
  influxSensor.addField("temperature", sensorTask.getTempReading());
  influxSensor.addField("humidity", sensorTask.getHumReading());
  influxSensor.addField("lightlevel", sensorTask.getLdrReading());
  if (!influxClient.writePoint(influxSensor))
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
  vTaskDelay(refreshTime / portTICK_PERIOD_MS);
}