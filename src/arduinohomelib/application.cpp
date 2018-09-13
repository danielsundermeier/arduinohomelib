#include "arduinohomelib/application.h"

EthernetClient ethClient;
PubSubClient pubSubClient(ethClient);

void Application::setup()
{
    componentsCount = components.size();

    Serial.begin(9600);

    Serial.print("Start ");
    Serial.println(Settings::name);

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
        //Component *component = this->components[i];
        //component->setup();
        this->components[i]->setup();
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
        //Component *component = this->components[i];
        //component->loop();
        this->components[i]->loop();
    }
}

void Application::handleMqttMessage(char* topic, byte* payload, unsigned int length)
{
    Serial.print("MQTT Received [");
    Serial.print(topic);
    Serial.print("] ");
    String cmd = "";
    for (unsigned int i=0;i<length;i++)
    {
        cmd += (char)payload[i];
    }
    Serial.println(cmd);
    for (uint32_t i = 0; i < componentsCount; i++) {
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

EthernetComponent *Application::initEthernet(byte* mac)
{
    auto *ethernet = new EthernetComponent(mac);
    this->ethernet = ethernet;

    return this->ethernet;
}

MqttClient *Application::initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length), void (*connectedCallback)())
{
    struct MQTTCredentials credentials { serverId, 1883, username, password, Settings::name };
    auto *mqtt = new MqttClient(pubSubClient, credentials, messageReceivedCallback, connectedCallback);
    this->mqtt = mqtt;

    return this->mqtt;
}

UdpComponent *Application::initUdp(void (*callback)(char*), IPAddress receiverIp)
{
    auto *udp = new UdpComponent(callback, receiverIp);
    this->udp = udp;

    return this->udp;
}

void Application::makeMomentaryButton(int pin)
{
    this->registerComponent(
        new MomentaryButton(pin)
    );
}

void Application::makeMomentaryButton(int pin, int relaisPin)
{
    this->registerComponent(
        new MomentaryButton(pin, relaisPin)
    );
}

void Application::makeSwitch(int pin)
{
    this->registerComponent(
        new Switch(pin)
    );
}

Application App;