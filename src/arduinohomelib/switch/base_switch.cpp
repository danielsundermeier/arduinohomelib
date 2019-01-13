#include "arduinohomelib/switch/base_switch.h"

BaseSwitch::BaseSwitch(const char* name) : Nameable(name) {}

BaseSwitch::BaseSwitch(const char* name, int pin) : Nameable(name)
{
    setPin(pin);
}

void BaseSwitch::setPin(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void BaseSwitch::on()
{
    write(HIGH);
}

void BaseSwitch::off()
{
    write(LOW);
}

void BaseSwitch::toggle()
{
    write(! read());
}

void BaseSwitch::write(int value)
{

}

int BaseSwitch::read()
{

}

void BaseSwitch::subscribe()
{
    globalMqttClient->subscribe(this->getTopic("set"));
}

void BaseSwitch::handleMqttConnected()
{
    if (this->useMqtt == false)
    {
        return;
    }

    this->subscribe();
    if (isDiscovered == false)
    {
        discover();
    }
}

void BaseSwitch::handleMqttMessage(String cmd)
{
    if (this->useMqtt == false)
    {
        return;
    }

    if (strcmp(cmd.c_str(), "ON") == 0)
    {
        on();
    }
    else if (strcmp(cmd.c_str(), "OFF") == 0)
    {
        off();
    }
}

void BaseSwitch::handleUdpMessage(int pin, const char* cmd)
{
    if (pin == this->pin)
    {
        if (strcmp(cmd, "") == 0)
        {
            this->toggle();
        }
        else if (strcmp(cmd, "on") == 0)
        {
            this->on();
        }
        else if (strcmp(cmd, "off") == 0)
        {
            this->off();
        }
        else if (strcmp(cmd, "toggle") == 0)
        {
            this->toggle();
        }
    }
}

void BaseSwitch::discover()
{
    StaticJsonBuffer<ARDOINOHOMELIB_JSON_BUFFER_SIZE> JSONbuffer;
    JsonObject& discoveryInfo = JSONbuffer.createObject();

    discoveryInfo["platform"] = "mqtt";
    discoveryInfo["name"] = this->getName();
    discoveryInfo["state_topic"] = this->getTopic("state");
    discoveryInfo["command_topic"] = this->getTopic("set");
    discoveryInfo["availability_topic"] = String(Settings::name) + "/status";

    if (globalMqttClient->publish(this->getDiscoveryTopic(), discoveryInfo) == true)
    {
        globalMqttClient->publish(this->getTopic("set"), "OFF");
        isDiscovered = true;
    }
    else
    {
        Logger->debug("switch", "Error sending discovery");
        Logger->debug("switch", "size message: %d", discoveryInfo.measureLength() + 1);
    }
}