#ifndef ARDUINOHOMELIB_MAX6675_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_MAX6675_SENSOR_COMPONENT_H

#include <max6675.h>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class Max6675Sensor : public Component
{
    public:
        Max6675Sensor(int so, int cs, int clk);
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