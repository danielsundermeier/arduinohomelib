#ifndef ARDUINOHOMELIB_MQTT_CLIENT_H
#define ARDUINOHOMELIB_MQTT_CLIENT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>

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
        MqttClient(PubSubClient client, const MQTTCredentials &credentials);
        void setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)());
        void setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length));
        void loop();
        bool connect();
        bool publish(const char* topic, const char* payload);
        bool publish(const char* topic, JsonObject& data);
        bool subscribe(const char* topic);
    private:
        bool isConnected = false;
        MQTTCredentials _credentials;
        PubSubClient client;
        void (*_connectedCallback)();
        String _statusTopic;
        void available();
        void unavailable();
};

#endif