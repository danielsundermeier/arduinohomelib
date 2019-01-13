#ifndef ARDUINOHOMELIB_I2C_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_I2C_SWITCH_COMPONENT_H

#include "arduinohomelib/defines.h"
#ifdef ARDUINOHOMELIB_USE_SWITCH_I2C

#include <Wire.h>
#include <Adafruit_MCP23017.h>

#include "arduinohomelib/switch/base_switch.h"

class I2CSwitch : public BaseSwitch
{
    public:
        I2CSwitch(const char* name, int pin, Adafruit_MCP23017 mcp);
        void setPin(int pin);
    protected:
        Adafruit_MCP23017 mcp;

        int read();
        void write(int value);
};

#endif

#endif