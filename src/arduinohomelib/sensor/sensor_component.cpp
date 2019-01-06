#include "arduinohomelib/sensor/sensor_component.h"

SensorComponent::SensorComponent(String name) : Nameable(name) {}

void SensorComponent::update() {}

void SensorComponent::handleInterval()
{
    this->update();
    Component::handleInterval();
}

void SensorComponent::discover()
{
    StaticJsonBuffer<ARDOINOHOMELIB_JSON_BUFFER_SIZE> JSONbuffer;
    JsonObject& discoveryInfo = JSONbuffer.createObject();

    discoveryInfo["platform"] = "mqtt";
    discoveryInfo["name"] = this->getName();
    discoveryInfo["unique_id"] = this->fullId;
    discoveryInfo["state_topic"] = this->stateTopic;
    discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
    discoveryInfo["expire_after"] = (this->getUpdateInterval() + 2) * (this->valuesSendCount / 1000);
    if (this->getDeviceClass() != "")
    {
        discoveryInfo["device_class"] = this->getDeviceClass();
    }
    if (this->getIcon() != "")
    {
        discoveryInfo["icon"] = this->getIcon();
    }
    if (this->getUnitOfMeassurement() != "")
    {
        discoveryInfo["unit_of_measurement"] = this->getUnitOfMeassurement();
    }

    if (globalMqttClient->publish(this->discoveryTopic.c_str(), discoveryInfo) == true)
    {
        this->setValueStr();
        this->sendValue();
        isDiscovered = true;
    }
    else
    {
        Logger->debug("sensor", "Error sending discovery for %s", this->discoveryTopic.c_str());
    }
}

void SensorComponent::handleMqttConnected()
{
    if (isDiscovered == false && this->useMqtt == true)
    {
        discover();
    }
}

bool SensorComponent::shouldSendValue()
{
    return (globalMqttClient != nullptr && this->valuesCount >= this->valuesSendCount);
}

void SensorComponent::resetValuesCount()
{
    this->valuesCount = 0;
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


EmptySensorComponent::EmptySensorComponent(String name, short unsigned int accuracyDecimals, String icon, String unitOfMeassurement) : SensorComponent(name)
{
    this->accuracyDecimals = accuracyDecimals;
    this->icon = icon;
    this->unitOfMeassurement = unitOfMeassurement;
}

void EmptySensorComponent::setup()
{
    Serial.print("freeMemory=");
    Serial.println(freeMemory());

    this->friendlyName = this->getName();
    Logger->debug("EmptySensor", "Name\t%s", this->friendlyName.c_str());
    Logger->debug("EmptySensor", "ID\t%s", this->getId().c_str());
    delay(1000);

    this->fullId = String(Settings::name) + "_" + this->getId();
    Logger->debug("EmptySensor", "Full ID\t%s", this->fullId.c_str());
    delay(2000);

    this->stateTopic = String(Settings::name) + "/" + this->getId() + "/state";
    Logger->debug("EmptySensor", "State Topic\t%s", this->stateTopic.c_str());
    delay(1000);

    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->getId() + "/config";
    Logger->debug("EmptySensor", "Discovery Topic\t%s", this->discoveryTopic.c_str());
    delay(1000);

    setDiscoveryInfo();

    Serial.print("freeMemory=");
    Serial.println(freeMemory());
}

void EmptySensorComponent::setNewRawValue(double rawValue)
{
    this->newRawValue(rawValue);
}