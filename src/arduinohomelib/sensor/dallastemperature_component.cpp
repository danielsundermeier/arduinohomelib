#include "arduinohomelib/sensor/dallastemperature_component.h"

DallasTemperatureSensor::DallasTemperatureSensor(int pin, OneWire oneWire) : SensorComponent("")
{
    this->pin = pin;
    this->onewire = &oneWire;
    this->sensors.setOneWire(&oneWire);
}

DS18B20* DallasTemperatureSensor::add(const char* name, DeviceAddress address)
{
    return this->registerComponent(new DS18B20(name, address));
}

void DallasTemperatureSensor::setup()
{
    this->componentsCount = this->components.size();

    byte addr[8];
    Logger->debug("DallasTemperatureSensor", "Suche Sensoren...");
    while(this->onewire->search(addr))
    {
        Logger->debug("DallasTemperatureSensor", "Gefundene Adresse: 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X.", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7], addr[8]);
    }
    this->onewire->reset_search();

    this->sensors.begin();

    this->setInterval(this->getUpdateInterval());

    for (uint32_t i = 0; i < componentsCount; i++) {
        this->components[i]->setup();
    }
}

void DallasTemperatureSensor::update()
{
    this->sensors.requestTemperatures();
    // Logger->debug("DallasTemperatureSensor", "Temperatur: %g", temp);
    delay(1000);
    for (uint32_t i = 0; i < this->componentsCount; i++) {
        double temp = this->sensors.getTempC(this->components[i]->address);
        this->components[i]->setNewRawValue(temp);
        if (this->components[i]->shouldSendValue())
        {
            Logger->debug("sensor.ds18b20", "Send Value: %s%s", this->components[i]->getValueStr().c_str(), this->components[i]->getUnitOfMeassurement().c_str());
            this->components[i]->sendValue();
            this->components[i]->resetValuesCount();
        }
        Logger->debug("sensor.ds18b20", "New Value: %s%s", this->components[i]->getRawValueStr().c_str(), this->components[i]->getUnitOfMeassurement().c_str());
    }
}

void DallasTemperatureSensor::discover()
{
    this->isDiscovered = true;
}

void DallasTemperatureSensor::setUpdateInterval(unsigned int updateInterval)
{

}

void DallasTemperatureSensor::setValuesSendCount(unsigned int valuesSendCount)
{

}

DS18B20* DallasTemperatureSensor::registerComponent(DS18B20* c)
{
    DS18B20* component = c;
    if (c != nullptr)
    {
        this->components.push_back(component);
    }

    return c;
}

DS18B20::DS18B20(const char* name, uint8_t* address) : EmptySensorComponent(name, 1, "", "°C")
{
    this->address = address;
    // Logger->debug("DallasTemperatureSensor", "Adresse: %02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X.", this->address[0], this->address[1], this->address[2], this->address[3], this->address[4], this->address[5], this->address[6], this->address[7], this->address[8]);
}