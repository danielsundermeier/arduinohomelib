#include "arduinohomelib/network/ethernet_component.h"

EthernetComponent::EthernetComponent(byte* mac)
{
    EthernetClient client;
    this->mac = mac;
}

void EthernetComponent::setup()
{
    Logger->debug("ethernet", "Setup");
    Ethernet.begin(this->mac);
    delay(1000);
    IPAddress ip = Ethernet.localIP();
    Logger->debug("ethernet", "IP\t%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

void EthernetComponent::loop()
{
    Ethernet.maintain();
}

EthernetClient EthernetComponent::getClient() const
{
    return client;
}