#include "arduinohomelib/switch/switch_component.h"

Switch::Switch(const char* name, int pin) : BaseSwitch(name, pin) {}

void Switch::write(int value)
{
    Logger->debug("switch", "Schalte\t[%2d]\t%d", this->pin, value);

    digitalWrite(this->pin, value);

    if (this->useMqtt == false)
    {
        return;
    }

    globalMqttClient->publish(this->getTopic("state"), (read() == 1 ? "ON" : "OFF"));
}

int Switch::read()
{
    return digitalRead(this->pin);
}