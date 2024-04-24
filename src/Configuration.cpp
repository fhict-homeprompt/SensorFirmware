#include "Configuration.h"

AppConfig defaultConfig = {
    .configVersion = 1,
    .mqttConfig = {
        .mqttBroker = NULL,
        .mqttTopic = NULL,
        .mqttPort = 1883,
        .keepAliveInterval = 30000},
    .sensorConfig = {
        .ldrTreshold = 1000,
        .ldrTriggerDelay = 5000,
        .ldrResetDelay = 5000,
    }};

void ConfigurationLoader::loadDefaultConfig(AppConfig *config)
{
    memcpy(config, &defaultConfig, sizeof(AppConfig));
    config->mqttConfig.mqttBroker = strdup("test.mosquitto.org");
    config->mqttConfig.mqttTopic = strdup("homeprompt/sensor/livingroom");
}

bool ConfigurationLoader::loadConfig(AppConfig *config)
{
    AppConfig configBuffer;
    File configFile;

    if (!SPIFFS.exists("/config"))
    {
        Serial.println("Config file does not exist");
        return false;
    }
    configFile = SPIFFS.open("/config", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }
    configFile.read((uint8_t *)&configBuffer, sizeof(AppConfig));
    if (configBuffer.configVersion != defaultConfig.configVersion)
    {
        Serial.println("Config version mismatch");
        return false;
    }
    memcpy(config, &configBuffer, sizeof(AppConfig));
    configFile.close();
    return true;
}

bool ConfigurationLoader::saveConfig(AppConfig *config)
{
    if (SPIFFS.exists("/config"))
    {
        SPIFFS.remove("/config");
    }
    File configFile = SPIFFS.open("/config", "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    configFile.write((uint8_t *)config, sizeof(AppConfig));
    configFile.close();
    return true;
}