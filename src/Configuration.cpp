#include "Configuration.h"

AppConfig defaultConfig = {
    .configVersion = 1,
    .mqttConfig = {
        .mqttBroker = NULL,
        .mqttTopic = NULL,
        .mqttPort = 1883,
    },
    .sensorConfig = {
        .ldrTreshold = 1000,
        .ldrTriggerDelay = 5000,
        .ldrResetDelay = 5000,
    }
};

void ConfigurationLoader::loadDefaultConfig(AppConfig *config)
{
    memcpy(config, &defaultConfig, sizeof(AppConfig));
    config->mqttConfig.mqttBroker = strdup("test.mosquitto.org");
    config->mqttConfig.mqttTopic = strdup("homeprompt/sensor/livingroom");
}

bool ConfigurationLoader::loadConfig(AppConfig *config)
{
    Serial.println("config: loadConfig unimplemented.");
    return false;
}