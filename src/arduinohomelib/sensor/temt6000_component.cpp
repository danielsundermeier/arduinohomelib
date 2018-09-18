#include "arduinohomelib/sensor/temt6000_component.h"

Temt6000Sensor::Temt6000Sensor(String name, int pin) : SensorComponent(name)
{
    this->pin = pin;
}

void Temt6000Sensor::setup()
{
    pinMode(pin, INPUT);

    this->setInterval("", this->getUpdateInterval(), this);

    this->friendlyName = this->getName();
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void Temt6000Sensor::update()
{
    this->newRawValue((analogRead(pin) / 10000.0) * 2000000.0);
    Logger->debug("sensor.temt6000", "New Value: %s", this->rawValueStr);

    if (this->valuesCount >= this->valuesSendCount)
    {
        Logger->debug("sensor.temt6000", "Send Value: %s", this->valueStr);
        this->sendValue();
        this->valuesCount = 0;
    }
}