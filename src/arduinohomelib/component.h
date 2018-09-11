#ifndef ARDUINOHOMELIB_COMPONENT_H
#define ARDUINOHOMELIB_COMPONENT_H

#include "arduinohomelib/settings.h"

class Component
{
    public:
        Component();
        virtual void setup();
        virtual void loop();
};

#endif