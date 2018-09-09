#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class Application
{
    private:
        String name = "No Name Set";
    public:
        EthernetComponent *ethernet{nullptr};
        EthernetComponent *initEthernet();
        MqttClient *initMqtt(String serverId, String username, String password);
        MqttClient *mqtt;
        void setName(const char* name);
        void setup();
        void loop();
};

extern Application App;

#endif