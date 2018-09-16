#include "arduinohomelib/sensor/temt6000_component.h"

Temt6000Sensor::Temt6000Sensor(int pin)
{
    this->pin = pin;
}

void Temt6000Sensor::setup()
{
    pinMode(pin, INPUT);

    this->setInterval("", this->getUpdateInterval(), this);

    this->friendlyName = "Helligkeit";
    this->id = "brightness_" + String(this->pin);
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void Temt6000Sensor::update()
{
    this->newRawValue((analogRead(pin) / 10000.0) * 2000000.0);

    if (this->valuesCount >= this->valuesSendCount)
    {
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.temt6000", "New Value: %s", this->rawValueStr);
}