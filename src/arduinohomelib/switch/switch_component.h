#ifndef ARDUINOHOMELIB_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_SWITCH_COMPONENT_H

#include "arduinohomelib/component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

const int BUFFER_SIZE = 300;

class Switch : public Component
{
    public:
        Switch();
        Switch(int pin);
        void setPin(int pin);
        void on();
        void off();
        void toggle();
        void subscribe();
        void discover();
        void handleMqttMessage(String cmd);
    private:
        bool isDiscovered = false;
        const char* _device = "switch";
        String _stateTopic;
        String _discoveryTopic;
        String _friendlyName;
        String _id;
        String _fullId;
        int _pin;
        StaticJsonBuffer<BUFFER_SIZE> JSONbuffer;
        JsonObject& _discoveryInfo = JSONbuffer.createObject();
        int read();
        void write(int value);
        void setDiscoveryInfo();
};

extern MqttClient *globalMqttClient;

#endif