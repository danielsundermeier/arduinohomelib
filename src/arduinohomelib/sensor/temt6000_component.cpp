#include "arduinohomelib/sensor/temt6000_component.h"

Temt6000Sensor::Temt6000Sensor(const char* name, int pin) : SensorComponent(name)
{
    this->pin = pin;

    this->unitOfMeassurement = "lux";
    this->setDeviceClass("illuminance");
}

void Temt6000Sensor::setup()
{
    pinMode(pin, INPUT);

    this->setInterval(this->getUpdateInterval());
}

void Temt6000Sensor::update()
{
    this->newRawValue((analogRead(pin) / 10000.0) * 2000000.0);
    Logger->debug("sensor.temt6000", "New Value: %s%s", this->rawValueStr, this->getUnitOfMeassurement().c_str());

    if (this->shouldSendValue())
    {
        Logger->debug("sensor.temt6000", "Send Value: %s%s", this->valueStr, this->getUnitOfMeassurement().c_str());
        this->sendValue();
        this->valuesCount = 0;
    }
}