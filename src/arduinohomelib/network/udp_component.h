#ifndef ARDUINOHOMELIB_UDP_COMPONENT_H
#define ARDUINOHOMELIB_UDP_COMPONENT_H

#include <Arduino.h>
#include <Ethernet.h>
#include "arduinohomelib/log_component.h"

class UdpComponent
{
    public:
        UdpComponent();
        UdpComponent(void (*callback)(char*), IPAddress ip);
        void setup();
        void setup(void (*callback)(char*));
        void loop();
        void send(const char* message);
        void send(IPAddress destinationIp, const char* message);
        void sendCommand(int pin, const char* command);
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

extern UdpComponent *globalUdpComponent;

#endif