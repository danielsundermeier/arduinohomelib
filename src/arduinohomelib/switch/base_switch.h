#ifndef ARDUINOHOMELIB_BASE_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_BASE_SWITCH_COMPONENT_H

#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class BaseSwitch : public Component, public Nameable
{
    public:
        BaseSwitch(const char* name);
        BaseSwitch(const char* name, int pin);
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

        virtual int read();
        virtual void write(int value);

        const char* getDevice() const { return "switch"; }
};

#endif