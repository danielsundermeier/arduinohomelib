#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include <MemoryFree.h>
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include "arduinohomelib/helpers.h"
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"
#include "arduinohomelib/network/udp_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

#include "arduinohomelib/binary_sensor/hcsr501_component.h"
#include "arduinohomelib/button/momentary_button_component.h"
#include "arduinohomelib/light/fastled_component.h"
#include "arduinohomelib/sensor/dht22_component.h"
#include "arduinohomelib/sensor/max31856_component.h"
#include "arduinohomelib/sensor/max6675_component.h"
#include "arduinohomelib/sensor/temt6000_component.h"
#include "arduinohomelib/switch/switch_component.h"

class Application
{
    private:
        String name = "No Name Set";
        uint32_t componentsCount = 0;
        template<class C>
        C* registerComponent(C* c);

    public:
        Application();
        std::vector<Component*> components{};
        LogComponent* log{nullptr};
        LogComponent* initLog();
        EthernetComponent* ethernet{nullptr};
        EthernetComponent* initEthernet(byte* mac);
        MqttClient* initMqtt(String serverId, String username, String password);
        MqttClient* initMqtt(String serverId, String username, String password, void (*connectedCallback)());
        MqttClient* initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length));
        MqttClient* initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        MqttClient* mqtt{nullptr};
        UdpComponent* udp{nullptr};
        UdpComponent* initUdp(void (*callback)(char*), IPAddress receiverIp);

        uint8_t getComponentsCount() const;

        void setName(const char* name);
        void setup();
        void loop();
        void handleMqttMessage(char* topic, byte* payload, unsigned int length);
        void handleMqttMessageHomeAssistantStatus(String cmd);

        void handleUdpMessage(char* message);

        // Binary_sensor
        Hcsr501BinarySensor* makeHcsr501BinarySensor(String name, int pin);

        // Button
        MomentaryButton* makeMomentaryButton(String name, int pin);
        MomentaryButton* makeMomentaryButton(String name, int pin, int relaisPin);

        // Light
        FastledLight* makeFastledLight(String name, int pin, unsigned short int numLeds, CRGB* leds);

        // Sensor
        Dht22Sensor* makeDht22Sensor(String nameTemperature, String nameHumidity, int pin);
        Max31856Sensor* makeMax31856Sensor(String name, int csPin);
        Max31856Sensor* makeMax31856Sensor(String name, int csPin, int diPin, int doPin, int clkPin);
        Max6675Sensor* makeMax6675Sensor(String name, int so, int cs, int clk);
        Temt6000Sensor* makeTemt6000Sensor(String name, int pin);

        // Switch
        Switch* makeSwitch(String name, int pin);
};

extern Application App;

template<class C>
C* Application::registerComponent(C* c) {
    Component* component = c;
    if (c != nullptr)
    {
        this->components.push_back(component);
    }

    return c;
}

void defaultMqttMessageReceived(char* topic, byte* payload, unsigned int length);
void defaultMqttConnected();

#endif