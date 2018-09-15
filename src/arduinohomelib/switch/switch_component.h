#ifndef ARDUINOHOMELIB_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_SWITCH_COMPONENT_H

#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

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
        void handleMqttConnected();

    protected:
        const char* device = "switch";
        int pin;

        int read();
        void write(int value);

        void setDiscoveryInfo();
};

#endif