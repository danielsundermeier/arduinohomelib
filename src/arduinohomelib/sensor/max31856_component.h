#ifndef ARDUINOHOMELIB_MAX31956_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_MAX31956_SENSOR_COMPONENT_H

#include <SPI.h>
#include <Adafruit_MAX31856.h>
#include "arduinohomelib/sensor/sensor_component.h"

class Max31856Sensor : public SensorComponent
{
    public:
        Max31856Sensor(const char* name, int csPin);
        Max31856Sensor(const char* name, int csPin, int diPin, int doPin, int clkPin);

        void setup() override;

        void update();

    protected:

        int diPin;
        int doPin;
        int clkPin;

        float value;

        Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(-1);
};

#endif