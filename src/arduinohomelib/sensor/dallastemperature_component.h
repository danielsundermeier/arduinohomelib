#ifndef ARDUINOHOMELIB_DALLASTEMPERATURE_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_DALLASTEMPERATURE_SENSOR_COMPONENT_H

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

        void setUpdateInterval(unsigned int updateInterval) override;
        void setValuesSendCount(unsigned int valuesSendCount) override;

        DS18B20* registerComponent(DS18B20* c);

    protected:
        int pin;
        uint32_t componentsCount = 0;

        DallasTemperature sensors;
        OneWire* onewire;

        std::vector<DS18B20*> components{};

        void updateTemperature();

        unsigned int getUpdateInterval() const override { return this->updateInterval; }

};

#endif