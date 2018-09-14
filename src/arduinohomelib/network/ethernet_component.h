#ifndef ARDUINOHOMELIB_ETHERNET_COMPONENT_H
#define ARDUINOHOMELIB_ETHERNET_COMPONENT_H

#include <Ethernet.h>
#include "arduinohomelib/log_component.h"

class EthernetComponent
{
    public:
        EthernetComponent(byte* mac);
        void setup();
        void loop();
        EthernetClient getClient() const;
    private:
        byte* mac;
        EthernetClient client;
};

#endif