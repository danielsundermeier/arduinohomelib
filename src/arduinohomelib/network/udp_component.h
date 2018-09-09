#ifndef ARDUINOHOMELIB_UDP_COMPONENT_H
#define ARDUINOHOMELIB_UDP_COMPONENT_H

#include <Arduino.h>
#include <Ethernet.h>

class UdpComponent
{
    public:
        UdpComponent();
        UdpComponent(IPAddress ip);
        void setup();
        void setup(void (*callback)(char*));
        void loop();
        void send(String message);
        void send(IPAddress destinationIp, String message);
        void responde();
        void responde(String message);
        void receive();
        void setCallback(void (*callback)(char*));
    private:
        EthernetUDP UdpClient;
        void (*_callback)(char*);
        IPAddress _sendToIp;
        char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
        int port = 8888;
        void defaultCallback(char* message);
};

#endif