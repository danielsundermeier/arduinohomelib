#ifndef ARDUINOHOMELIB_COMPONENT_H
#define ARDUINOHOMELIB_COMPONENT_H

#include <Arduino.h>
#include "arduinohomelib/settings.h"

class Component
{
    public:
        Component();
        virtual void setup();
        virtual void loop();
        virtual void on();
        virtual void off();
        virtual void subscribe();
        virtual String getCommandTopic();
        virtual void handleMqttMessage(String cmd);
    protected:
        String _commandTopic;
};

#endif