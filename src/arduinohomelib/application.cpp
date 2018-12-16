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

    Serial.print("freeMemory=");
    Serial.println(freeMemory());
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

void Application::handleMqttMessageHomeAssistantStatus(String cmd)
{
    if (strcmp(cmd.c_str(), "online") == 0)
    {
        globalMqttClient->available();
        for (uint32_t i = 0; i < App.getComponentsCount(); i++) {
            App.components[i]->undiscovered();
            App.components[i]->discover();
        }
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
    if (strcmp(topic, "hass/status") == 0)
    {
        this->handleMqttMessageHomeAssistantStatus(cmd);

        return;
    }

    for (uint32_t i = 0; i < componentsCount; i++)
    {
        if (strcmp(topic, App.components[i]->getCommandTopic().c_str()) == 0)
        {
            this->components[i]->handleMqttMessage(cmd);
        }
    }
}

void Application::handleUdpMessage(char* message)
{

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

MqttClient* Application::initMqtt(String serverId, String username, String password)
{
    return this->initMqtt(serverId, username, password, defaultMqttMessageReceived, defaultMqttConnected);
}

MqttClient* Application::initMqtt(String serverId, String username, String password, void (*connectedCallback)())
{
    return this->initMqtt(serverId, username, password, defaultMqttMessageReceived, connectedCallback);
}

MqttClient* Application::initMqtt(String serverId, String username, String password, void (*messageReceivedCallback)(char* topic, byte* payload, unsigned int length))
{
    return this->initMqtt(serverId, username, password, messageReceivedCallback, defaultMqttConnected);
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

FastledLight* Application::makeFastledLight(String name, int pin, unsigned short int numLeds, CRGB* leds)
{
    return this->registerComponent(
        new FastledLight(name, pin, numLeds, leds)
    );
}

Dht22Sensor* Application::makeDht22Sensor(String nameTemperature, String nameHumidity, int pin)
{
    return this->registerComponent(
        new Dht22Sensor(nameTemperature, nameHumidity, pin)
    );
}

Max31856Sensor* Application::makeMax31856Sensor(String name, int csPin)
{
    return this->registerComponent(
        new Max31856Sensor(name, csPin)
    );
}

Max31856Sensor* Application::makeMax31856Sensor(String name, int csPin, int diPin, int doPin, int clkPin)
{
    return this->registerComponent(
        new Max31856Sensor(name, csPin, diPin, doPin, clkPin)
    );
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

void defaultMqttMessageReceived(char* topic, byte* payload, unsigned int length)
{
    App.handleMqttMessage(topic, payload, length);
}

void defaultMqttConnected()
{
    for (uint32_t i = 0; i < App.getComponentsCount(); i++) {
        App.components[i]->handleMqttConnected();
    }
}