#ifndef ARDUINOHOMELIB_ETHERNETCOMPONENT_H
#define ARDUINOHOMELIB_ETHERNETCOMPONENT_H

#include <Ethernet.h>

class EthernetComponent
{
    public:
        EthernetComponent();
        void setup();
        void loop();
        EthernetClient getClient() const;
    private:
        byte mac[24] { 0xFE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
        EthernetClient client;
        byte* generateMac();
};

#endif