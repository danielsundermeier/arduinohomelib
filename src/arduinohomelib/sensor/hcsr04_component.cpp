#include "arduinohomelib/sensor/hcsr04_component.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_HCSR04

Hcsr04Sensor::Hcsr04Sensor(const char* name, int triggerPin, int echoPin) : SensorComponent(name)
{
    this->pin = triggerPin;
    this->echoPin = echoPin;
    this->unitOfMeassurement = "cm";
}

void Hcsr04Sensor::setup()
{
    pinMode(this->pin, OUTPUT);
    pinMode(this->echoPin,INPUT);

    this->setInterval(this->getUpdateInterval());
}

void Hcsr04Sensor::update()
{
    this->newRawValue(this->ranging());
    if (this->shouldSendValue())
    {
        Logger->debug("sensor.hcsr04", "Send Value: %s%s", this->valueStr, this->getUnitOfMeassurement().c_str());
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.hcsr04", "New Value: %s%s", this->rawValueStr, this->getUnitOfMeassurement().c_str());
}

long Hcsr04Sensor::timing()
{
    digitalWrite(this->pin, LOW);
    delayMicroseconds(2);
    digitalWrite(this->pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->pin, LOW);
    long duration = pulseIn(this->echoPin, HIGH, this->timeout);
    if ( duration == 0 ) {
        duration = this->timeout;
    }

    return duration;
}

long Hcsr04Sensor::ranging()
{
    return this->timing() / 29 / 2 ;
}

#endif