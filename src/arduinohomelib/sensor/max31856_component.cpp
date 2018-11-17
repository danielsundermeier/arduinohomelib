#include "arduinohomelib/sensor/max31856_component.h"

Max31856Sensor::Max31856Sensor(String name, int csPin) : SensorComponent(name)
{
    this->csPin = csPin;

    this->unitOfMeassurement = "°C";
}

Max31856Sensor::Max31856Sensor(String name, int csPin, int diPin, int doPin, int clkPin) : SensorComponent(name)
{
    this->csPin = csPin;
    this->diPin = diPin;
    this->doPin = doPin;
    this->clkPin = clkPin;

    this->unitOfMeassurement = "°C";
}

void Max31856Sensor::setup()
{
    this->thermocouple = Adafruit_MAX31856(this->csPin, this->diPin, this->doPin, this->clkPin);
    this->thermocouple.begin();
    this->thermocouple.setThermocoupleType(this->thermocouple.getThermocoupleType());
    delay(500);

    this->setInterval(this->getUpdateInterval());

    this->friendlyName = this->getName();
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->csPin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void Max31856Sensor::update()
{
    this->newRawValue(this->thermocouple.readThermocoupleTemperature());
    if (this->shouldSendValue())
    {
        Logger->debug("sensor.max31856", "Send Value: %s%s", this->valueStr, this->getUnitOfMeassurement().c_str());
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.max31856", "New Value: %s%s", this->rawValueStr, this->getUnitOfMeassurement().c_str());
}