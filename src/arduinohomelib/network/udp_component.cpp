#include "arduinohomelib/network/udp_component.h"

UdpComponent::UdpComponent()
{
    _callback = NULL;
    globalUdpComponent = this;
}

UdpComponent::UdpComponent(void (*callback)(char*), IPAddress ip)
{
    _sendToIp = ip;
    setCallback(callback);

    globalUdpComponent = this;
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
        IPAddress ip = UdpClient.remoteIP();
        Logger->debug("udp", "Received\t[%d.%d.%d.%d]\t%s", ip[0], ip[1], ip[2], ip[3], packetBuffer);

        if (_callback != NULL)
        {
            _callback(packetBuffer);
        }
    }
}

void UdpComponent::sendCommand(int pin, const char* command)
{
    char message [15];
    sprintf(message, "%d:%s", pin, command);
    send(_sendToIp, message);
}

void UdpComponent::send(const char* message)
{
    send(_sendToIp, message);
}

void UdpComponent::send(IPAddress destinationIp, const char* message)
{
    Logger->debug("udp", "Send\t[%d.%d.%d.%d]\t%s", destinationIp[0], destinationIp[1], destinationIp[2], destinationIp[3], message);

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

UdpComponent *globalUdpComponent = nullptr;