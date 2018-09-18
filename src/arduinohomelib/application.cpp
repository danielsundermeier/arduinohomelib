#include "arduinohomelib/application.h"

EthernetClient ethClient;
PubSubClient pubSubClient(ethClient);

Application::Application()
{
    randomSeed(analogRead(0));
}

void Application::setup()
{
    componentsCount = components.size();

    Logger->debug("application", "Start %s", Settings::name);

    if (this->ethernet != nullptr)
    {
        this->ethernet->setup();
    }

    if (this->mqtt != nullptr)
    {
        this->mqtt->setup();
    }

    if (this->udp != nullptr)
    {
        this->udp->setup();
    }

    for (uint32_t i = 0; i < componentsCount; i++) {
        this->components[i]->setup_();
    }
}

void Application::loop()
{
    if (this->ethernet != nullptr)
    {
        this->ethernet->loop();
    }

    if (this->mqtt != nullptr)
    {
        this->mqtt->loop();
    }

    if (this->udp != nullptr)
    {
        this->udp->loop();
    }

    for (uint32_t i = 0; i < componentsCount; i++) {
        this->components[i]->loop_();
    }
}

void Application::handleMqttMessage(char* topic, byte* payload, unsigned int length)
{
    String cmd = "";
    for (unsigned int i=0;i<length;i++)
    {
        cmd += (char)payload[i];
    }
    Logger->debug("mqtt", "Received\t[%s]\t%s", topic, cmd.c_str());
    for (uint32_t i = 0; i < componentsCount; i++)
    {
        if (strcmp(topic, App.components[i]->getCommandTopic().c_str()) == 0)
        {
            this->components[i]->handleMqttMessage(cmd);
        }
    }
}

uint8_t Application::getComponentsCount() const
{
    return componentsCount;
}

void Application::setName(const char* name)
{
    Settings::name = name;
}

LogComponent* Application::initLog()
{
    this->log = new LogComponent();

    return this->log;
}

EthernetComponent* Application::initEthernet(byte* mac)
{
    this->ethernet = new EthernetComponent(mac);

    return this->ethernet;
}

MqttClient* Application::initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)())
{
    struct MQTTCredentials credentials { serverId, 1883, username, password, Settings::name };
    this->mqtt = new MqttClient(pubSubClient, credentials, messageReceivedCallback, connectedCallback);

    return this->mqtt;
}

UdpComponent* Application::initUdp(void (*callback)(char*), IPAddress receiverIp)
{
    auto* udp = new UdpComponent(callback, receiverIp);
    this->udp = udp;

    return this->udp;
}

Hcsr501BinarySensor* Application::makeHcsr501BinarySensor(String name, int pin)
{
    return this->registerComponent(
        new Hcsr501BinarySensor(name, pin)
    );
}

MomentaryButton* Application::makeMomentaryButton(String name, int pin)
{
    return this->registerComponent(
        new MomentaryButton(name, pin)
    );
}

MomentaryButton* Application::makeMomentaryButton(String name, int pin, int relaisPin)
{
    return this->registerComponent(new MomentaryButton(name, pin, relaisPin));
}

Max6675Sensor* Application::makeMax6675Sensor(String name, int so, int cs, int clk)
{
    return this->registerComponent(
        new Max6675Sensor(name, so, cs, clk)
    );
}

Temt6000Sensor* Application::makeTemt6000Sensor(String name, int pin)
{
    return this->registerComponent(
        new Temt6000Sensor(name, pin)
    );
}

Switch* Application::makeSwitch(String name, int pin)
{
    return this->registerComponent(
        new Switch(name, pin)
    );
}

Application App;