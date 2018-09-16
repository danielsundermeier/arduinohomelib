#include "arduinohomelib/sensor/temt6000_component.h"

Temt6000Sensor::Temt6000Sensor(int pin)
{
    this->pin = pin;
}

void Temt6000Sensor::setup()
{
    pinMode(pin, INPUT);

    this->setInterval("", this->getUpdateInterval(), this);

    this->friendlyName = "Helligkeit";
    this->id = "brightness_" + String(this->pin);
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void Temt6000Sensor::handleInterval()
{
    this->update();
}

void Temt6000Sensor::update()
{
    //this->value = analogRead(this->pin);
    this->rawValue = (analogRead(pin) / 10000.0) * 2000000.0;
    this->newRawValue(this->rawValue);

    if (this->valuesCount >= this->valuesSendCount)
    {
        this->sendValue();
        this->valuesCount = 0;
    }
    Logger->debug("sensor.temt6000", "New Value: %s", this->valueStr);
}

void Temt6000Sensor::discover()
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

void Temt6000Sensor::handleMqttConnected()
{
    if (isDiscovered == false)
    {
        discover();
    }
}

void Temt6000Sensor::sendValue()
{
    Serial.print("Send Value: ");
    Serial.println(this->value);
    globalMqttClient->publish(this->stateTopic.c_str(), this->valueStr);
}

void Temt6000Sensor::newRawValue(double rawValue)
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

double Temt6000Sensor::calculateAverage()
{
    if (this->values.size() == 0)
    {
        return 0;
    }

    return this->sum / this->values.size();
}

void Temt6000Sensor::setDiscoveryInfo()
{
    this->discoveryInfo["platform"] = "mqtt";
    this->discoveryInfo["device_class"] = "illuminance";
    this->discoveryInfo["name"] = this->friendlyName;
    this->discoveryInfo["unique_id"] = this->fullId;
    this->discoveryInfo["state_topic"] = this->stateTopic;
    this->discoveryInfo["availability_topic"] = String(Settings::name) + "/status";
    this->discoveryInfo["expire_after"] = this->getUpdateInterval() * this->valuesSendCount / 1000;
}

void Temt6000Sensor::setUpdateInterval(int updateInterval)
{
    this->updateInterval = updateInterval;
}

int Temt6000Sensor::getUpdateInterval() const
{
    return this->updateInterval;
}