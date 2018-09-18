#include "arduinohomelib/binary_sensor/hcsr501_component.h"

BinarySensor::BinarySensor(String name, int pin) : Nameable(name)
{
    this->pin = pin;
}

void BinarySensor::setup()
{
    pinMode(this->pin, INPUT);

    this->friendlyName = "Bewegung";
    this->id = "motion_" + String(this->pin);
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void BinarySensor::loop()
{
    state = digitalRead(this->pin);
    if (this->toHigh())
    {
        this->sendState();
        Logger->debug("binary_sensor.hcsr501", "Bewegung erkannt");
    }
    if (this->toLow())
    {
        this->sendState();
        Logger->debug("binary_sensor.hcsr501", "Keine Bewegung erkannt");
    }
    lastState = state;
}

bool BinarySensor::toHigh()
{
    return (this->state == HIGH && this->lastState == LOW);
}

bool BinarySensor::toLow()
{
    return (this->state == LOW && this->lastState == HIGH);
}

void BinarySensor::handleMqttConnected()
{
    if (isDiscovered == false)
    {
        discover();
    }
}

void BinarySensor::discover()
{
    if (globalMqttClient->publish(this->discoveryTopic.c_str(), this->discoveryInfo) == true)
    {
        this->sendState();
        isDiscovered = true;
    }
    else
    {
        Logger->debug("binary_sensor", "Error sending discovery");
    }
}

void BinarySensor::sendState()
{
    globalMqttClient->publish(this->stateTopic.c_str(), (this->state ? "ON" : "OFF"));
}

const char* BinarySensor::getDeviceClass() const
{
    return "None";
}

void BinarySensor::setDiscoveryInfo()
{
    this->discoveryInfo["platform"] = "mqtt";
    this->discoveryInfo["device_class"] = this->getDeviceClass();
    this->discoveryInfo["name"] = this->getName();
    this->discoveryInfo["unique_id"] = this->fullId;
    this->discoveryInfo["state_topic"] = this->stateTopic;
    this->discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
}

