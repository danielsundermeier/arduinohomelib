#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"
#include "arduinohomelib/network/udp_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"
#include "arduinohomelib/button/momentary_button_component.h"

class Application
{
    private:
        String name = "No Name Set";
    public:
        EthernetComponent *ethernet{nullptr};
        EthernetComponent *initEthernet();
        MqttClient *initMqtt(String serverId, String username, String password);
        MqttClient *mqtt{nullptr};
        UdpComponent *udp{nullptr};
        UdpComponent *initUdp(IPAddress receiverIp);
        void setName(const char* name);
        void setup();
        void loop();
};

extern Application App;

#endif