#ifndef ARDUINOHOMELIB_MQTT_CLIENT_H
#define ARDUINOHOMELIB_MQTT_CLIENT_H

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "arduinohomelib/log_component.h"

struct MQTTCredentials {
    String serverIp; ///< The address of the server without port number
    uint16_t port; ///< The port number of the server.
    String username;
    String password;
    String clientId; ///< The client ID. Will automatically be truncated to 23 characters.
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
        String _statusTopic;
        String _debugTopic;
        void unavailable();
};

extern MqttClient *globalMqttClient;

#endif