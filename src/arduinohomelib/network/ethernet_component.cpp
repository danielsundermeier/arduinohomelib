#include "arduinohomelib/network/ethernet_component.h"

EthernetComponent::EthernetComponent(byte* mac)
{
    EthernetClient client;
    this->mac = mac;
}

void EthernetComponent::setup()
{
    Serial.println("Ethernet Setup");
    Ethernet.begin(this->mac);
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