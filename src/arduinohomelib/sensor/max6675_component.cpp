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
    // does not work
    /*
    this->setInterval("update", 2000, [this]() {
        this->update();
    });


    // works
    this->setInterval("update", 2000, []() {
        Serial.println("Test");
    });
    */
}

void Max6675Sensor::update()
{
    this->value = this->thermocouple.readCelsius();
    Logger->debug("sensor.max6675", "Neuer Wert: %f", this->value);
}