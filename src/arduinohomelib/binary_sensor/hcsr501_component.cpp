#include "arduinohomelib/binary_sensor/hcsr501_component.h"

Hcsr501BinarySensor::Hcsr501BinarySensor(String name, int pin) : BinarySensor(name, pin) {}

const char* Hcsr501BinarySensor::getDeviceClass() const
{
    return "motion";
}