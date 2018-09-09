#include "arduinohomelib/network/udp_component.h"

UdpComponent::UdpComponent()
{
    _callback = NULL;
}

UdpComponent::UdpComponent(IPAddress ip)
{
    _callback = NULL;
    _sendToIp = ip;
}

void UdpComponent::setup()
{
    UdpClient.begin(port);
}

void UdpComponent::setup(void (*callback)(char*))
{
    setCallback(callback);
    UdpClient.begin(port);
}

void UdpComponent::loop()
{
    receive();
}

void UdpComponent::receive()
{
    int packetSize = UdpClient.parsePacket();
    if (packetSize)
    {
        memset(packetBuffer, 0, sizeof(packetBuffer));
        UdpClient.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.print("Nachricht erhalten: ");
        Serial.println(packetBuffer);
        if (_callback != NULL)
        {
            _callback(packetBuffer);
        }
    }
}

void UdpComponent::send(String message)
{
    send(_sendToIp, message);
}

void UdpComponent::send(IPAddress destinationIp, String message)
{
    UdpClient.beginPacket(destinationIp, port);
    UdpClient.print(message);
    UdpClient.endPacket();
    delay(10);
}

void UdpComponent::responde()
{
    responde("1");
}

void UdpComponent::responde(String message)
{
    UdpClient.beginPacket(UdpClient.remoteIP(), UdpClient.remotePort());
    UdpClient.print(message);
    UdpClient.endPacket();
    delay(10);
}

void UdpComponent::setCallback(void (*callback)(char*))
{
    _callback = callback;
}