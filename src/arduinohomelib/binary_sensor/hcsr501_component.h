#ifndef ARDUINOHOMELIB_HCSR501_BINARY_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_HCSR501_BINARY_SENSOR_COMPONENT_H

#include "arduinohomelib/binary_sensor/binary_sensor.h"

class Hcsr501BinarySensor : public BinarySensor
{
    public:
        Hcsr501BinarySensor(int pin);
    protected:
        const char* getDeviceClass() const;
};

#endif