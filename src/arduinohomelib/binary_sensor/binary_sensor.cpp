#include "arduinohomelib/binary_sensor/hcsr501_component.h"

BinarySensor::BinarySensor(String name, int pin) : Nameable(name)
{
    this->pin = pin;
}

void BinarySensor::setup()
{
    pinMode(this->pin, INPUT);

    this->friendlyName = "Bewegung";
    this->id = "motion_" + String(this->pin);
    this->fullId = String(Settings::name) + "_" + this->id;

    this->stateTopic = String(Settings::name) + "/" + String(this->pin) + "/state";
    this->discoveryTopic = String(Settings::mqttDiscoveryPrefix) + "/" + this->device +"/" + this->fullId + "/" + this->id + "/config";

    setDiscoveryInfo();
}

void BinarySensor::loop()
{
    this->state = digitalRead(this->pin);
    if (this->toHigh())
    {
        this->stateCallback.call(this->state);
        this->sendState();
        Logger->debug("binary_sensor.hcsr501", "Bewegung erkannt");
    }
    if (this->toLow())
    {
        this->stateCallback.call(this->state);
        this->sendState();
        Logger->debug("binary_sensor.hcsr501", "Keine Bewegung erkannt");
    }
    lastState = state;
}

bool BinarySensor::toHigh()
{
    return (this->state == HIGH && this->lastState == LOW);
}

bool BinarySensor::toLow()
{
    return (this->state == LOW && this->lastState == HIGH);
}

void BinarySensor::handleMqttConnected()
{
    if (this->isDiscovered == false && this->useMqtt == true)
    {
        discover();
    }
}

void BinarySensor::discover()
{
    StaticJsonBuffer<ARDOINOHOMELIB_JSON_BUFFER_SIZE> JSONbuffer;
    JsonObject& discoveryInfo = JSONbuffer.createObject();

    discoveryInfo["platform"] = "mqtt";
    discoveryInfo["device_class"] = this->getDeviceClass();
    discoveryInfo["name"] = this->getName();
    discoveryInfo["unique_id"] = this->fullId;
    discoveryInfo["state_topic"] = this->stateTopic;
    discoveryInfo["availability_topic"] = String(Settings::name) + "/status";

    if (globalMqttClient->publish(this->discoveryTopic.c_str(), discoveryInfo) == true)
    {
        this->sendState();
        isDiscovered = true;
    }
    else
    {
        Logger->debug("binary_sensor", "Error sending discovery");
    }

    JSONbuffer.clear();
}

void BinarySensor::sendState()
{
    if (this->useMqtt == false)
    {
        return;
    }

    globalMqttClient->publish(this->stateTopic.c_str(), (this->state ? "ON" : "OFF"));
}

const char* BinarySensor::getDeviceClass() const
{
    return "None";
}

void BinarySensor::setDiscoveryInfo()
{

}

void BinarySensor::addStateCallback(void (*function)(bool state))
{
    this->stateCallback.add(function);
}

