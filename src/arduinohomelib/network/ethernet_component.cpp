#include "arduinohomelib/network/ethernet_component.h"

EthernetComponent::EthernetComponent()
{
    EthernetClient client;
}

void EthernetComponent::setup(byte* mac)
{
    Serial.println("Ethernet Setup");
    Ethernet.begin(mac);
    delay(1500);
    Serial.println(Ethernet.localIP());
}

void EthernetComponent::loop()
{
    Ethernet.maintain();
}

EthernetClient EthernetComponent::getClient() const
{
    return client;
}