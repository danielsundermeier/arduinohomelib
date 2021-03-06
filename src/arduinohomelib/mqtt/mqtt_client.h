#ifndef ARDUINOHOMELIB_MQTT_CLIENT_H
#define ARDUINOHOMELIB_MQTT_CLIENT_H

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "arduinohomelib/log_component.h"

struct MQTTCredentials {
    const char* serverIp; ///< The address of the server without port number
    uint16_t port; ///< The port number of the server.
    const char* username;
    const char* password;
    const char* clientId; ///< The client ID. Will automatically be truncated to 23 characters.
};

class MqttClient
{
    public:
        MqttClient(PubSubClient client, const MQTTCredentials &credentials, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        void setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        void setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length));
        void setup();
        void loop();
        bool connect();
        bool publish(const char* topic, const char* payload);
        bool publish(const char* topic, JsonObject& data);
        bool subscribe(const char* topic);
        bool log(const char* payload);

        void available();

        void setConnectedCallback(void (*connectedCallback)());
        void setMessageReceivedCallback(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length));
    private:
        bool isConnected = false;
        MQTTCredentials _credentials;
        PubSubClient client;
        void (*_connectedCallback)();
        void unavailable();
};

extern MqttClient *globalMqttClient;

#endif