#include "arduinohomelib/component.h"

Component::Component() {}

void Component::setup_()
{
    this->setup();
}

void Component::loop_()
{
    this->loopInternal();

    this->loop();
}

void Component::setup()
{

}

void Component::loop()
{

}

void Component::undiscovered()
{
    this->isDiscovered = false;
}

void Component::discover()
{

}

void Component::subscribe()
{

}

String Component::getCommandTopic()
{
    return this->commandTopic;
}

void Component::handleMqttConnected()
{

}

void Component::handleMqttMessage(String cmd)
{

}

void Component::setDiscoveryInfo()
{

}

bool Component::cancelTimeFunction(const std::string &name, TimeFunction::Type type)
{
    if (name.empty())
    {
        return false;
    }
    for (auto iter = this->timeFunctions.begin(); iter != this->timeFunctions.end(); iter++) {
        if (!iter->remove && iter->name == name && iter->type == type)
        {
            iter->remove = true;

            return true;
        }
    }

    return false;
}

bool Component::cancelInterval(const std::string &name)
{
    return this->cancelTimeFunction(name, TimeFunction::INTERVAL);
}

void Component::setInterval(uint32_t interval, const std::string &name)
{
    int offset = 0;
    if (interval != 0)
    {
        offset = (random(65535) % interval) / 2;
    }
    this->cancelInterval(name);
    struct TimeFunction function { name, TimeFunction::INTERVAL, interval, millis() - interval - offset, this, false };
    this->timeFunctions.push_back(function);
}

void Component::handleInterval()
{
    this->intervalCallbacks.call();
}

void Component::handleTimeout()
{
    this->timeoutCallbacks.call();
}

bool Component::cancelTimeout(const std::string &name)
{
    return this->cancelTimeFunction(name, TimeFunction::TIMEOUT);
}

void Component::setTimeout(uint32_t timeout, const std::string &name)
{
    this->cancelTimeout(name);
    struct TimeFunction function { name, TimeFunction::TIMEOUT, timeout, millis(), this, false };
    this->timeFunctions.push_back(function);
}

void Component::loopInternal()
{
    for (unsigned int i = 0; i < this->timeFunctions.size(); i++)
    {
        const uint32_t now = millis();
        TimeFunction *tf = &this->timeFunctions[i];
        if (tf->shouldRun(now))
        {
            if (tf->type == TimeFunction::INTERVAL)
            {
                tf->c->handleInterval();
            }
            else if (tf->type == TimeFunction::TIMEOUT)
            {
                tf->c->handleTimeout();
            }

            tf = &this->timeFunctions[i];

            if (tf->type == TimeFunction::INTERVAL && tf->interval != 0)
            {
                const uint32_t amount = (now - tf->last_execution) / tf->interval;
                tf->last_execution += (amount * tf->interval);
            }
            else if (tf->type == TimeFunction::DEFER || tf->type == TimeFunction::TIMEOUT)
            {
                tf->remove = true;
            }
        }
    }

    this->timeFunctions.erase(
        std::remove_if(this->timeFunctions.begin(), this->timeFunctions.end(),
            [](const TimeFunction &tf) -> bool {
                return tf.remove;
            }
        ),
        this->timeFunctions.end()
    );
}

bool Component::TimeFunction::shouldRun(uint32_t now) const
{
    if (this->remove)
    {
        return false;
    }
    if (this->type == DEFER)
    {
        return true;
    }

    return this->interval != 4294967295UL && now - this->last_execution > this->interval;
}

void Component::addIntervalCallback(void (*function)())
{
    this->intervalCallbacks.add(function);
}

void Component::addTimeoutCallback(void (*function)())
{
    this->timeoutCallbacks.add(function);
}

Nameable::Nameable(String name)
{
    setName(name);
}

void Nameable::setName(String name)
{
    this->name = name;
    this->id = toKebabCase(name);
}