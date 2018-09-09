#ifndef ARDUINOHOMELIB_APPLICATION_H
#define ARDUINOHOMELIB_APPLICATION_H

#include <Arduino.h>
#include "arduinohomelib/settings.h"
#include "arduinohomelib/network/ethernet_component.h"

class Application
{
    private:
        String name = "No Name Set";
        EthernetComponent *ethernet{nullptr};
        EthernetComponent *initEthernet();
    public:
        void setName(const char* name);
        void setup();
};

extern Application App;

#endif