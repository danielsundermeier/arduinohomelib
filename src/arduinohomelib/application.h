#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include <MemoryFree.h>
#include <vector>

#include "arduinohomelib/defines.h"
#include "arduinohomelib/helpers.h"
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"
#include "arduinohomelib/network/udp_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

#ifdef ARDUINOHOMELIB_USE_BINARY_SENSOR_MOTION
    #include "arduinohomelib/binary_sensor/hcsr501_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_BUTTON_MOMENTARY
    #include "arduinohomelib/button/momentary_button_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_LIGHT_FASTLED
    #include "arduinohomelib/light/fastled_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_DALLAS
    #include "arduinohomelib/sensor/dallastemperature_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_DHT
    #include "arduinohomelib/sensor/dht22_component.h"
#endif

#include "arduinohomelib/sensor/hcsr04_component.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_MAX31856
    #include "arduinohomelib/sensor/max31856_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_MAX6675
    #include "arduinohomelib/sensor/max6675_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_TEMT6000
    #include "arduinohomelib/sensor/temt6000_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SWITCH
    #include "arduinohomelib/switch/switch_component.h"
#endif

#ifdef ARDUINOHOMELIB_USE_SWITCH_I2C
    #include "arduinohomelib/switch/i2c_switch_component.h"
#endif

class Application
{
    private:
        const char* name = "No Name Set";
        uint32_t componentsCount = 0;


    public:
        Application();
        std::vector<Component*> components{};
        LogComponent* log{nullptr};
        LogComponent* initLog();
        EthernetComponent* ethernet{nullptr};
        EthernetComponent* initEthernet(byte* mac);
        MqttClient* initMqtt(const char* serverId, const char* username, const char* password);
        MqttClient* initMqtt(const char* serverId, const char* username, const char* password, void (*connectedCallback)());
        MqttClient* initMqtt(const char* serverId, const char* username, const char* password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length));
        MqttClient* initMqtt(const char* serverId, const char* username, const char* password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        MqttClient* mqtt{nullptr};
        UdpComponent* udp{nullptr};
        UdpComponent* initUdp(void (*callback)(char*), IPAddress receiverIp);

        // Binary_sensor
        #ifdef ARDUINOHOMELIB_USE_BINARY_SENSOR_MOTION
            Hcsr501BinarySensor* makeHcsr501BinarySensor(const char* name, int pin);
        #endif
        // Button
        #ifdef ARDUINOHOMELIB_USE_BUTTON_MOMENTARY
            MomentaryButton* makeMomentaryButton(const char* name, int pin);
            MomentaryButton* makeMomentaryButton(const char* name, int pin, int relaisPin);
        #endif

        // Light
        #ifdef ARDUINOHOMELIB_USE_LIGHT_FASTLED
            FastledLight* makeFastledLight(const char* name, int pin, unsigned short int numLeds, CRGB* leds);
        #endif
        // Sensor
        #ifdef ARDUINOHOMELIB_USE_SENSOR_DHT
            Dht22Sensor* makeDht22Sensor(const char* nameTemperature, const char* nameHumidity, int pin);
        #endif

        #ifdef ARDUINOHOMELIB_USE_SENSOR_MAX31856
            Max31856Sensor* makeMax31856Sensor(const char* name, int csPin);
            Max31856Sensor* makeMax31856Sensor(const char* name, int csPin, int diPin, int doPin, int clkPin);
        #endif

        #ifdef ARDUINOHOMELIB_USE_SENSOR_MAX6675
            Max6675Sensor* makeMax6675Sensor(const char* name, int so, int cs, int clk);
        #endif

        #ifdef ARDUINOHOMELIB_USE_SENSOR_TEMT6000
            Temt6000Sensor* makeTemt6000Sensor(const char* name, int pin);
        #endif

        // Switch
        #ifdef ARDUINOHOMELIB_USE_SWITCH
            Switch* makeSwitch(const char* name, int pin);
        #endif

        template<class C>
        C* registerComponent(C* c);

        uint8_t getComponentsCount() const;

        void setName(const char* name);
        void setup();
        void loop();
        void handleMqttMessage(char* topic, byte* payload, unsigned int length);
        void handleMqttMessageHomeAssistantStatus(String cmd);

        void handleUdpMessage(char* message);
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