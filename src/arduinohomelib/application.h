#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include "arduinohomelib/component.h"
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"
#include "arduinohomelib/network/udp_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"
#include "arduinohomelib/button/momentary_button_component.h"

class Application
{
    private:
        String name = "No Name Set";
        std::vector<Component*> components{};

        template<class C>
        C *registerComponent(C *c);

    public:
        EthernetComponent *ethernet{nullptr};
        EthernetComponent *initEthernet(byte* mac);
        MqttClient *initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        MqttClient *mqtt{nullptr};
        UdpComponent *udp{nullptr};
        UdpComponent *initUdp(void (*callback)(char*), IPAddress receiverIp);
        void setName(const char* name);
        void setup();
        void loop();
        void makeMomentaryButton(int pin);
        void makeMomentaryButton(int pin, int relaisPin);
};

extern Application App;

template<class C>
C *Application::registerComponent(C *c) {
    Component *component = c;
    if (c != nullptr)
    {
        this->components.push_back(component);
    }

    return c;
}

#endif