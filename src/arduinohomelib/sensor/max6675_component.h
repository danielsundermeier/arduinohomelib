#ifndef ARDUINOHOMELIB_MAX6675_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_MAX6675_SENSOR_COMPONENT_H

#include <SPI.h>
#include "max6675.h"
#include "arduinohomelib/sensor/sensor_component.h"

class Max6675Sensor : public SensorComponent
{
    public:
        Max6675Sensor(const char* name, int soPin, int csPin, int clkPin);

        void setup() override;

        void update();

    protected:
        int soPin;
        int csPin;
        int clkPin;

        float value;

        MAX6675 thermocouple;
};

#endif