#ifndef ARDUINOHOMELIB_I2C_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_I2C_SWITCH_COMPONENT_H

#include <Wire.h>
#include <Adafruit_MCP23017.h>

#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class I2CSwitch : public Component, public Nameable
{
    public:
        I2CSwitch(const char* name, int pin, Adafruit_MCP23017 mcp);
        void setPin(int pin);

        void on();
        void off();
        void toggle();

        void subscribe();
        void discover();
        void handleMqttMessage(String cmd);
        void handleMqttConnected();

        void handleUdpMessage(int pin, const char* cmd) override;

        int read();

    protected:
        const char* device = "switch";
        Adafruit_MCP23017 mcp;
        void write(int value);

        const char* getDevice() const { return "switch"; }
};

#endif