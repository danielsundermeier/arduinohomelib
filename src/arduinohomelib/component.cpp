#include "arduinohomelib/component.h"

Component::Component() {}

void Component::setup()
{

}

void Component::loop()
{

}

void Component::on()
{

}

void Component::off()
{

}

void Component::subscribe()
{

}

void Component::handleMqttMessage(String cmd)
{
    Serial.println(cmd);
}

String Component::getCommandTopic()
{
    return _commandTopic;
}