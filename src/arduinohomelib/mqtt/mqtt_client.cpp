#include "arduinohomelib/mqtt/mqtt_client.h"

MqttClient::MqttClient(PubSubClient client, const MQTTCredentials &credentials, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)()) : _credentials(credentials)
{
    this->client = client;
    this->client.setServer(_credentials.serverIp, _credentials.port);
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

    this->client.setServer(_credentials.serverIp, _credentials.port);
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setup(void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length))
{
    _connectedCallback = NULL;
    Logger->debug("mqtt", "Server\t%s", _credentials.serverIp);
    this->client.setServer(_credentials.serverIp, _credentials.port);
    this->client.setCallback(messageReceivedCallback);
}

void MqttClient::setup()
{
    Logger->debug("mqtt", "Server\t%s", _credentials.serverIp);
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
    char statusTopic [50];
    sprintf (statusTopic, "%s%s/status", Settings::mqttPrefix, _credentials.clientId);

    if (client.connect(_credentials.clientId, _credentials.username, _credentials.password, statusTopic, 1, 1, "offline"))
    {
        Logger->debug("mqtt", "Verbunden");
        if (_connectedCallback != NULL)
        {
           _connectedCallback();
        }
        available();
        subscribe("hass/status");
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

    return client.publish(topic, payload, true);
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
    char debugTopic [50];
    sprintf (debugTopic, "%s%s/debug", Settings::mqttPrefix, _credentials.clientId);

    return publish(debugTopic, payload);
}

void MqttClient::available()
{
    char statusTopic [50];
    sprintf (statusTopic, "%s%s/status", Settings::mqttPrefix, _credentials.clientId);

    publish(statusTopic, "online");
}

void MqttClient::unavailable()
{
    char statusTopic [50];
    sprintf (statusTopic, "%s%s/status", Settings::mqttPrefix, _credentials.clientId);

    publish(statusTopic, "offline");
}

MqttClient *globalMqttClient = nullptr;
