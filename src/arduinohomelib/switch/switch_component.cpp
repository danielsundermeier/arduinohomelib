#include "arduinohomelib/switch/switch_component.h"

Switch::Switch() {}

Switch::Switch(int pin)
{
    setPin(pin);
}

void Switch::setPin(int pin)
{
    _pin = pin;
    pinMode(pin, OUTPUT);

    _friendlyName = "Switch " + String(_pin);
    _id = "Switch_" + String(_pin);
    _fullId = String(Settings::name) + "_" + _id;

    _commandTopic = String(Settings::name) + "/" + String(_pin) + "/set";
    _stateTopic = String(Settings::name) + "/" + String(_pin) + "/state";
    _discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + _device +"/" + _fullId + "/" + _id + "/config";

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
    Serial.print("Schalte Switch ");
    Serial.print(_pin);
    Serial.print(" ");
    Serial.println(value);
    digitalWrite(_pin, value);
    globalMqttClient->publish(_stateTopic.c_str(), (read() == 1 ? "ON" : "OFF"));
}

int Switch::read()
{
    return digitalRead(_pin);
}

void Switch::subscribe()
{
    globalMqttClient->subscribe(_commandTopic.c_str());
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
    _discoveryInfo["platform"] = "mqtt";
    _discoveryInfo["name"] = _friendlyName;
    _discoveryInfo["state_topic"] = _stateTopic;
    _discoveryInfo["command_topic"] = _commandTopic;
    _discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
}

void Switch::discover()
{
    if (globalMqttClient->publish(_discoveryTopic.c_str(), _discoveryInfo) == true)
    {
        globalMqttClient->publish(_commandTopic.c_str(), "OFF");
        isDiscovered = true;
    }
    else
    {
        Serial.println("Error sending discovery");
    }
}