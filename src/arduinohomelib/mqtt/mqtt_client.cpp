#include "arduinohomelib/mqtt/mqtt_client.h"

MqttClient::MqttClient(PubSubClient client, const MQTTCredentials &credentials, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)()) : _credentials(credentials)
{

    _statusTopic = _credentials.clientId + "/status";
    this->client = client;
    this->client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    setConnectedCallback(connectedCallback);
    setMessageReceivedCallback(messageReceivedCallback);

    globalMqttClient = this;
}

void MqttClient::setMessageReceivedCallback(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length))
{
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setConnectedCallback(void (*connectedCallback)())
{
    _connectedCallback = connectedCallback;
}

void MqttClient::setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)())
{
    _connectedCallback = connectedCallback;

    this->client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length))
{
    _connectedCallback = NULL;
    Serial.print("MQTT Server: ");
    Serial.println(_credentials.serverIp);
    this->client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setup()
{
    Serial.print("MQTT Server: ");
    Serial.println(_credentials.serverIp);
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
           _connectedCallback();
        }
        available();
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

MqttClient *globalMqttClient = nullptr;
