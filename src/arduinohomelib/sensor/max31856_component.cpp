#include "arduinohomelib/sensor/max31856_component.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_MAX31856

Max31856Sensor::Max31856Sensor(const char* name, int csPin) : SensorComponent(name)
{
    this->pin = csPin;
    this->unitOfMeassurement = "°C";
}

Max31856Sensor::Max31856Sensor(const char* name, int csPin, int diPin, int doPin, int clkPin) : SensorComponent(name)
{
    this->pin = csPin;
    this->diPin = diPin;
    this->doPin = doPin;
    this->clkPin = clkPin;

    this->unitOfMeassurement = "°C";
}

void Max31856Sensor::setup()
{
    this->thermocouple = Adafruit_MAX31856(this->pin, this->diPin, this->doPin, this->clkPin);
    this->thermocouple.begin();
    this->thermocouple.setThermocoupleType(this->thermocouple.getThermocoupleType());
    delay(500);

    this->setInterval(this->getUpdateInterval());
}

void Max31856Sensor::update()
{
    this->newRawValue(this->thermocouple.readCJTemperature());
    if (this->shouldSendValue())
    {
        Logger->debug("sensor.max31856", "Send Value: %s%s", this->valueStr, this->getUnitOfMeassurement().c_str());
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.max31856", "New Value: %s%s", this->rawValueStr, this->getUnitOfMeassurement().c_str());
}

#endif