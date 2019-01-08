#include "arduinohomelib/sensor/sensor_component.h"

SensorComponent::SensorComponent(const char* name) : Nameable(name) {}

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
    discoveryInfo["unique_id"] = this->getFullId();
    discoveryInfo["state_topic"] = this->getTopic("state");
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

    if (globalMqttClient->publish(this->getDiscoveryTopic(), discoveryInfo) == true)
    {
        this->setValueStr();
        this->sendValue();
        isDiscovered = true;
    }
    else
    {
        Logger->debug("sensor", "Error sending discovery for %s", this->getDiscoveryTopic());
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
    globalMqttClient->publish(this->getTopic("state"), this->valueStr);
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


EmptySensorComponent::EmptySensorComponent(const char* name, short unsigned int accuracyDecimals, String icon, String unitOfMeassurement) : SensorComponent(name)
{
    this->accuracyDecimals = accuracyDecimals;
    this->icon = icon;
    this->unitOfMeassurement = unitOfMeassurement;
}

void EmptySensorComponent::setup()
{
    Serial.print("freeMemory=");
    Serial.println(freeMemory());

    Logger->debug("EmptySensor", "Name\t%s", this->getName());
    Logger->debug("EmptySensor", "ID\t%s", this->getId().c_str());
    delay(1000);

    Logger->debug("EmptySensor", "Full ID\t%s", this->getFullId());
    delay(2000);

    Logger->debug("EmptySensor", "State Topic\t%s", this->getTopic("state"));
    delay(1000);

    Logger->debug("EmptySensor", "Discovery Topic\t%s", this->getDiscoveryTopic());
    delay(1000);

    Serial.print("freeMemory=");
    Serial.println(freeMemory());
}

void EmptySensorComponent::setNewRawValue(double rawValue)
{
    this->newRawValue(rawValue);
}

char* EmptySensorComponent::getTopic(const char* suffix)
{
    sprintf (this->buffer, "%s/%s/%s", Settings::name, this->getId().c_str(), suffix);

    return this->buffer;
}