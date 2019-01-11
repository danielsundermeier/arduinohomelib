#ifndef ARDUINOHOMELIB_DALLASTEMPERATURE_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_DALLASTEMPERATURE_SENSOR_COMPONENT_H

#include "arduinohomelib/defines.h"

#ifdef ARDUINOHOMELIB_USE_SENSOR_DALLAS

#include <OneWire.h>
#include <DallasTemperature.h>

#include "arduinohomelib/sensor/sensor_component.h"

class DS18B20 : public EmptySensorComponent
{
    public:
        DS18B20(const char* name, uint8_t* address);

        uint8_t* address;
    protected:
};

class DallasTemperatureSensor : public SensorComponent
{
    public:
        DallasTemperatureSensor(int pin, OneWire oneWire);

        DS18B20* add(const char* name, DeviceAddress address);

        void setup() override;
        void update() override;

        void discover() override;

        DS18B20* registerComponent(DS18B20* c);

    protected:
        int pin;
        uint32_t componentsCount = 0;

        DallasTemperature sensors;
        OneWire* onewire;

        std::vector<DS18B20*> components{};
};

#endif

#endif