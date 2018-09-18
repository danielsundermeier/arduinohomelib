#include "arduinohomelib/switch/switch_component.h"

Switch::Switch(String name) : Nameable(name) {}

Switch::Switch(String name, int pin) : Nameable(name)
{
    setPin(pin);
}

void Switch::setPin(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);

    this->friendlyName = "Switch " + String(this->pin);
    this->id = "Switch_" + String(this->pin);
    this->fullId = String(Settings::name) + "_" + this->id;

    this->commandTopic = String(Settings::name) + "/" + String(this->pin) + "/set";
    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
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
    globalMqttClient->publish(this->stateTopic.c_str(), (read() == 1 ? "ON" : "OFF"));
}

int Switch::read()
{
    return digitalRead(this->pin);
}

void Switch::subscribe()
{
    globalMqttClient->subscribe(this->commandTopic.c_str());
}

void Switch::handleMqttConnected()
{
    this->subscribe();
    if (isDiscovered == false)
    {
        discover();
    }
}

void Switch::handleMqttMessage(String cmd)
{
    if (strcmp(cmd.c_str(), "ON") == 0)
    {
        on();
    }
    else if (strcmp(cmd.c_str(), "OFF") == 0)
    {
        off();
    }
}

void Switch::setDiscoveryInfo()
{
    this->discoveryInfo["platform"] = "mqtt";
    this->discoveryInfo["name"] = this->getName();
    this->discoveryInfo["state_topic"] = this->stateTopic;
    this->discoveryInfo["command_topic"] = this->commandTopic;
    this->discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
}

void Switch::discover()
{
    if (globalMqttClient->publish(this->discoveryTopic.c_str(), this->discoveryInfo) == true)
    {
        globalMqttClient->publish(this->commandTopic.c_str(), "OFF");
        isDiscovered = true;
    }
    else
    {
        Logger->debug("switch", "Error sending discovery");
    }
}