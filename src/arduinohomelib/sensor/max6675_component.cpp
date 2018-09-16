#include "arduinohomelib/sensor/max6675_component.h"

Max6675Sensor::Max6675Sensor(int so, int cs, int clk)
{
    this->soPin = soPin;
    this->csPin = csPin;
    this->clkPin = clkPin;
}

void Max6675Sensor::setup()
{
    thermocouple.begin(this->clkPin, this->csPin, this->soPin);

    this->setInterval("", 2000, this);
}

void Max6675Sensor::handleInterval()
{
    this->update();
}

void Max6675Sensor::update()
{
    this->value = this->thermocouple.readCelsius();
    Serial.println(this->value);
    Logger->debug("sensor.max6675", "Neuer Wert: %f", this->value);
}