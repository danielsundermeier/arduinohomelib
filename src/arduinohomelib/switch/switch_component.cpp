#include "arduinohomelib/switch/switch_component.h"

Switch::Switch(const char* name) : Nameable(name) {}

Switch::Switch(const char* name, int pin) : Nameable(name)
{
    setPin(pin);
}

void Switch::setPin(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Switch::on()
{
    write(HIGH);
}

void Switch::off()
{
    write(LOW);
}

void Switch::toggle()
{
    write(! read());
}

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

void Switch::subscribe()
{
    globalMqttClient->subscribe(this->getTopic("set"));
}

void Switch::handleMqttConnected()
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

void Switch::handleMqttMessage(String cmd)
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

void Switch::handleUdpMessage(int pin, const char* cmd)
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

void Switch::discover()
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
    }
}