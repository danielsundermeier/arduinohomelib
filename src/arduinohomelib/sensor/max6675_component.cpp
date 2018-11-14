#include "arduinohomelib/sensor/max6675_component.h"

Max6675Sensor::Max6675Sensor(String name, int soPin, int csPin, int clkPin) : SensorComponent(name)
{
    this->soPin = soPin;
    this->csPin = csPin;
    this->clkPin = clkPin;

    this->unitOfMeassurement = "°C";
}

void Max6675Sensor::setup()
{
    this->thermocouple.begin(this->clkPin, this->csPin, this->soPin);
    delay(500);

    this->setInterval(this->getUpdateInterval());

    this->friendlyName = this->getName();
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->clkPin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void Max6675Sensor::update()
{
    this->newRawValue(this->thermocouple.readCelsius());
    if (this->shouldSendValue())
    {
        Logger->debug("sensor.max6675", "Send Value: %s%s", this->valueStr, this->getUnitOfMeassurement().c_str());
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.max6675", "New Value: %s%s", this->rawValueStr, this->getUnitOfMeassurement().c_str());
}