#include "arduinohomelib/mqtt/mqtt_client.h"

MqttClient::MqttClient(PubSubClient client, const MQTTCredentials &credentials, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)()) : _credentials(credentials)
{

    _debugTopic = _credentials.clientId + "/debug";
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
    Logger->debug("mqtt", "Server\t%s", _credentials.serverIp.c_str());
    this->client.setServer(_credentials.serverIp.c_str(), _credentials.port);
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setup()
{
    Logger->debug("mqtt", "Server\t%s", _credentials.serverIp.c_str());
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
        Logger->debug("mqtt", "Verbunden");
        if (_connectedCallback != NULL)
        {
           _connectedCallback();
        }
        available();
        subscribe("homeassistant/status");
    }
    else
    {
        Logger->debug("mqtt", "Nicht verbunden");
    }

    isConnected = client.connected();

    return isConnected;
}

bool MqttClient::publish(const char* topic, const char* payload)
{
    Logger->debug("mqtt", "Publish\t[%s]\t%s", topic, payload);

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
    Logger->debug("mqtt", "Subscribed\t[%s]", topic);

    return client.subscribe(topic);
}

bool MqttClient::log(const char* payload)
{
    return publish(_debugTopic.c_str(), payload);
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
