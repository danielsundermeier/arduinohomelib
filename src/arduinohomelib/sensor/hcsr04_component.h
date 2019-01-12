#ifndef ARDUINOHOMELIB_HCSR04_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_HCSR04_SENSOR_COMPONENT_H

#include "arduinohomelib/defines.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_HCSR04

#include "arduinohomelib/sensor/sensor_component.h"

class Hcsr04Sensor : public SensorComponent
{
    public:
        Hcsr04Sensor(const char* name, int triggerPin, int echoPin);

        void setup() override;

        void update();

        long timing();
        long ranging();

    protected:
        int soPin;
        int echoPin;

        long timeout = 15000;
};

#endif

#endif