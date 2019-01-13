#include "arduinohomelib/switch/i2c_switch_component.h"

#ifdef ARDUINOHOMELIB_USE_SWITCH_I2C

I2CSwitch::I2CSwitch(const char* name, int pin, Adafruit_MCP23017 mcp) : BaseSwitch(name)
{
    this->mcp = mcp;
    this->setPin(pin);
}

void I2CSwitch::setPin(int pin)
{
    this->pin = pin;
    this->mcp.pinMode(this->pin, OUTPUT);
}

void I2CSwitch::write(int value)
{
    Logger->debug("I2CSwitch", "Schalte\t[%2d]\t%d", this->pin, value);

    this->mcp.digitalWrite(this->pin, value);

    if (this->useMqtt == false)
    {
        return;
    }

    globalMqttClient->publish(this->getTopic("state"), (read() == 1 ? "ON" : "OFF"));
}

int I2CSwitch::read()
{
    return this->mcp.digitalRead(this->pin);
}

#endif