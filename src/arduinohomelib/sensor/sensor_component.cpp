#include "arduinohomelib/sensor/sensor_component.h"

SensorComponent::SensorComponent() {}

void SensorComponent::update()
{

}

void SensorComponent::handleInterval()
{
    this->update();
}

void SensorComponent::discover()
{
    if (globalMqttClient->publish(this->discoveryTopic.c_str(), this->discoveryInfo) == true)
    {
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
    Serial.print("Send Value: ");
    Serial.println(this->value);
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
    this->valueStr[0] = '\0';
    dtostrf(this->value, 4, 2, &this->valueStr[strlen(this->valueStr)]);
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
    this->discoveryInfo["device_class"] = this->getDeviceClass();
    this->discoveryInfo["name"] = this->friendlyName;
    this->discoveryInfo["unique_id"] = this->fullId;
    this->discoveryInfo["state_topic"] = this->stateTopic;
    this->discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
    this->discoveryInfo["expire_after"] = this->getUpdateInterval() * this->valuesSendCount / 1000;
}

void SensorComponent::setUpdateInterval(int updateInterval)
{
    this->updateInterval = updateInterval;
}

int SensorComponent::getUpdateInterval() const
{
    return this->updateInterval;
}

void SensorComponent::setDeviceClass(String deviceClass)
{
    this->deviceClass = deviceClass;
}

String SensorComponent::getDeviceClass() const
{
    return this->deviceClass;
}