#include "arduinohomelib/sensor/dht22_component.h"

Dht22Sensor::Dht22Sensor(const char* nameTemperature, const char* nameHumidity, int pin) : SensorComponent("")
{
    this->pin = pin;

    this->temperatureSensor = new EmptySensorComponent(nameTemperature, 1, "", "°C");
    this->humiditySensor = new EmptySensorComponent(nameHumidity, 0, "mdi:water-percent", "%");
}

void Dht22Sensor::setup()
{
    dht.setup(this->pin);

    this->setInterval(this->getUpdateInterval());

    this->temperatureSensor->setup();
    this->humiditySensor->setup();
}

void Dht22Sensor::update()
{
    updateTemperature();
    updateHumidity();
}

void Dht22Sensor::updateTemperature()
{
    Serial.println(this->temperatureSensor->getName());
    this->temperatureSensor->setNewRawValue(this->dht.getTemperature());
    if (this->temperatureSensor->shouldSendValue())
    {
        Logger->debug("sensor.dhtTemp", "Send Value: %s%s", this->temperatureSensor->getValueStr().c_str(), this->temperatureSensor->getUnitOfMeassurement().c_str());
        this->temperatureSensor->sendValue();
        this->temperatureSensor->resetValuesCount();
    }
    Logger->debug("sensor.dhtTemp", "New Value: %s%s", this->temperatureSensor->getRawValueStr().c_str(), this->temperatureSensor->getUnitOfMeassurement().c_str());
}

void Dht22Sensor::updateHumidity()
{
    Serial.println(this->humiditySensor->getName());
    this->humiditySensor->setNewRawValue(this->dht.getHumidity());
    if (this->humiditySensor->shouldSendValue())
    {
        Logger->debug("sensor.dhtHum", "Send Value: %s%s", this->humiditySensor->getValueStr().c_str(), this->humiditySensor->getUnitOfMeassurement().c_str());
        this->humiditySensor->sendValue();
        this->humiditySensor->resetValuesCount();
    }
    Logger->debug("sensor.dhtHum", "New Value: %s%s", this->humiditySensor->getRawValueStr().c_str(), this->humiditySensor->getUnitOfMeassurement().c_str());
}

void Dht22Sensor::discover()
{
    this->temperatureSensor->discover();
    this->humiditySensor->discover();

    isDiscovered = true;
}

void Dht22Sensor::setUpdateInterval(unsigned int updateInterval)
{
    this->temperatureSensor->setUpdateInterval(updateInterval);
    this->humiditySensor->setUpdateInterval(updateInterval);
}

void Dht22Sensor::setValuesSendCount(unsigned int valuesSendCount)
{
    this->temperatureSensor->setValuesSendCount(valuesSendCount);
    this->humiditySensor->setValuesSendCount(valuesSendCount);
}