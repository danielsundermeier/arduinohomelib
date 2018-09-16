#ifndef ARDUINOHOMELIB_TEMT6000_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_TEMT6000_SENSOR_COMPONENT_H

#include "arduinohomelib/sensor/sensor_component.h"

class Temt6000Sensor : public SensorComponent
{
    public:
        Temt6000Sensor(int pin);

        void setup() override;

        void update() override;

    protected:
        String deviceClass = "illuminance";

        int pin;

        String getDeviceClass() const override { return this->deviceClass; }
};

#endif