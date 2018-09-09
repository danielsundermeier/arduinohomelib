#include "arduinohomelib/mqtt/mqtt_client.h"

MqttClient::MqttClient(PubSubClient client, const MQTTCredentials &credentials) : _credentials(credentials)
{
    _statusTopic = _credentials.clientId + "/status";
    this->client = client;
}

void MqttClient::setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)())
{
    _connectedCallback = connectedCallback;

    client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    client.setCallback(messageReceivedCallback);
}

void MqttClient::setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length))
{
    _connectedCallback = NULL;
    Serial.print("MQTT Server: ");
    Serial.println(_credentials.serverIp);
    client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    client.setCallback(messageReceivedCallback);
}

void MqttClient::loop()
{
    isConnected = client.connected();
    if (! isConnected)
    {
        connect();

    }
    client.loop();
}

bool MqttClient::connect()
{
    if (client.connect(_credentials.clientId.c_str(), _credentials.username.c_str(), _credentials.password.c_str(), _statusTopic.c_str(), 1, 0, "offline"))
    {
        Serial.println("MQTT verbunden");
        if (_connectedCallback != NULL)
        {
           available();
           _connectedCallback();
        }
    }
    else
    {
        Serial.println("MQTT nicht verbunden");
    }

    isConnected = client.connected();

    return isConnected;
}

bool MqttClient::publish(const char* topic, const char* payload)
{
    Serial.print("MQTT Publish [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println(payload);

    return client.publish(topic, payload);
}

bool MqttClient::publish(const char* topic, JsonObject& data)
{
    char payload[data.measureLength() + 1];
    data.printTo(payload, sizeof(payload));

    return publish(topic, payload);
}

bool MqttClient::subscribe(const char* topic)
{
    Serial.print("MQTT Subscribe [");
    Serial.print(topic);
    Serial.println("]");

    return client.subscribe(topic);
}

void MqttClient::available()
{
    publish(_statusTopic.c_str(), "online");
}

void MqttClient::unavailable()
{
    publish(_statusTopic.c_str(), "offline");
}
