#ifndef ARDUINOHOMELIB_SETTINGS_H
#define ARDUINOHOMELIB_SETTINGS_H

class Settings
{
    public:
        static const char* name;
        static const char* mqttPrefix; // Prefix muss mit / enden, falls vorhanden
        static const char* mqttDiscoveryPrefix;
};

#endif