#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>

struct AppConfig
{
    int configVersion;
    struct MqttConfig
    {
        char *mqttBroker;
        char *mqttTopic;
        int mqttPort;
    } mqttConfig;
    struct SensorConfig
    {
        int ldrTreshold;
        int ldrTriggerDelay;
        int ldrResetDelay;
    } sensorConfig;
};

class ConfigurationLoader
{
public:
    bool loadConfig(AppConfig *config);
    void loadDefaultConfig(AppConfig *config);
};

const unsigned int refreshTime = 1000;

// Pin definitions
const int pinDHT = 32;
const int pinLDR = 35;
const int pinLED = 14;
const int pinBTN = 27;

// Internal constants that maybe do not belong here
const unsigned int taskLowPriority = 1;
const unsigned int taskMediumPriority = 2;
const unsigned int taskHighPriority = 3;

#endif