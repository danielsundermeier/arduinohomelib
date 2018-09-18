#include "arduinohomelib/sensor/sensor_component.h"

SensorComponent::SensorComponent(String name) : Nameable(name) {}

void SensorComponent::update() {}

void SensorComponent::handleInterval()
{
    this->update();
}

void SensorComponent::discover()
{
    if (globalMqttClient->publish(this->discoveryTopic.c_str(), this->discoveryInfo) == true)
    {
        this->setValueStr();
        this->sendValue();
        isDiscovered = true;
    }
    else
    {
        Logger->debug("sensor", "Error sending discovery");
    }
}

void SensorComponent::handleMqttConnected()
{
    if (isDiscovered == false)
    {
        discover();
    }
}

void SensorComponent::sendValue()
{
    globalMqttClient->publish(this->stateTopic.c_str(), this->valueStr);
}

void SensorComponent::newRawValue(double rawValue)
{
    this->valuesCount++;
    this->rawValue = rawValue;
    if (this->values.size() == this->valuesMaxSize)
    {
        this->sum -= this->values.front();
        this->values.pop();
    }
    this->values.push(rawValue);
    this->sum += rawValue;

    this->value = calculateAverage();
    this->setRawValueStr();
    this->setValueStr();
}

double SensorComponent::calculateAverage()
{
    if (this->values.size() == 0)
    {
        return 0;
    }

    return this->sum / this->values.size();
}

void SensorComponent::setDiscoveryInfo()
{
    this->discoveryInfo["platform"] = "mqtt";
    this->discoveryInfo["name"] = this->getName();
    this->discoveryInfo["unique_id"] = this->fullId;
    this->discoveryInfo["state_topic"] = this->stateTopic;
    this->discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
    this->discoveryInfo["expire_after"] = this->getUpdateInterval() * this->valuesSendCount / 1000;
    if (this->getDeviceClass() != "")
    {
        this->discoveryInfo["device_class"] = this->getDeviceClass();
    }
    if (this->getIcon() != "")
    {
        this->discoveryInfo["icon"] = this->getIcon();
    }
    if (this->getUnitOfMeassurement() != "")
    {
        this->discoveryInfo["unit_of_measurement"] = this->getUnitOfMeassurement();
    }
}

void SensorComponent::setValueStr()
{
    this->valueStr[0] = '\0';
    dtostrf(this->value, 4, this->getAccuracyDecimals(), &this->valueStr[strlen(this->valueStr)]);
}

void SensorComponent::setRawValueStr()
{
    this->rawValueStr[0] = '\0';
    dtostrf(this->rawValue, 4, this->getAccuracyDecimals(), &this->rawValueStr[strlen(this->rawValueStr)]);
}

void SensorComponent::setUpdateInterval(unsigned int updateInterval)
{
    this->updateInterval = updateInterval;
}

void SensorComponent::setAccuracyDecimals(short unsigned int accuracyDecimals)
{
    this->accuracyDecimals = accuracyDecimals;
}

void SensorComponent::setDeviceClass(String deviceClass)
{
    this->deviceClass = deviceClass;
}

void SensorComponent::setIcon(String icon)
{
    this->icon = icon;
}

void SensorComponent::setUnitOfMeassurement(String unitOfMeassurement)
{
    this->unitOfMeassurement = unitOfMeassurement;
}

void SensorComponent::setValuesSendCount(unsigned int valuesSendCount)
{
    this->valuesSendCount = valuesSendCount;
}