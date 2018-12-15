#ifndef ARDUINOHOMELIB_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_SWITCH_COMPONENT_H

#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class Switch : public Component, public Nameable
{
    public:
        Switch(String name);
        Switch(String name, int pin);
        void setPin(int pin);

        void on();
        void off();
        void toggle();

        void subscribe();
        void discover();
        void handleMqttMessage(String cmd);
        void handleMqttConnected();

        void handleUdpMessage(int pin, const char* cmd) override;

    protected:
        const char* device = "switch";
        int pin;

        int read();
        void write(int value);

        void setDiscoveryInfo();
};

#endif