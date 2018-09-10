#ifndef ARDUINOHOMELIB_ETHERNET_COMPONENT_H
#define ARDUINOHOMELIB_ETHERNET_COMPONENT_H

#include <Ethernet.h>

class EthernetComponent
{
    public:
        EthernetComponent();
        void setup(byte* mac);
        void loop();
        EthernetClient getClient() const;
    private:
        EthernetClient client;
};

#endif