#ifndef ARDUINOHOMELIB_DHT22_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_DHT22_SENSOR_COMPONENT_H

#include "arduinohomelib/defines.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_DHT

#include <DHT.h>
#include "arduinohomelib/sensor/sensor_component.h"

class Dht22Sensor : public SensorComponent
{
    public:
        Dht22Sensor(const char* nameTemperature, const char* nameHumidity, int pin);

        void setup() override;
        void update() override;

        void discover() override;

        void setUpdateInterval(unsigned int updateInterval) override;
        void setValuesSendCount(unsigned int valuesSendCount) override;

    protected:
        int pin;

        DHT dht;

        EmptySensorComponent* temperatureSensor;
        EmptySensorComponent* humiditySensor;

        void updateTemperature();
        void updateHumidity();

        unsigned int getUpdateInterval() const override { return this->updateInterval; }
};

#endif

#endif