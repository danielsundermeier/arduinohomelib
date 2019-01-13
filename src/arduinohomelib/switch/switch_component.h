#ifndef ARDUINOHOMELIB_SWITCH_COMPONENT_H
#define ARDUINOHOMELIB_SWITCH_COMPONENT_H

#include "arduinohomelib/switch/base_switch.h"

class Switch : public BaseSwitch
{
    public:
        Switch(const char* name, int pin);
    protected:
        int read();
        void write(int value);
};

#endif