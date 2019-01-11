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
        if (strcmp(topic, App.components[i]->getTopic("set")) == 0)
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

#ifdef ARDUINOHOMELIB_USE_BINARY_SENSOR_MOTION
Hcsr501BinarySensor* Application::makeHcsr501BinarySensor(const char* name, int pin)
{
    return this->registerComponent(
        new Hcsr501BinarySensor(name, pin)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_BUTTON_MOMENTARY
MomentaryButton* Application::makeMomentaryButton(const char* name, int pin)
{
    return this->registerComponent(
        new MomentaryButton(name, pin)
    );
}

MomentaryButton* Application::makeMomentaryButton(const char* name, int pin, int relaisPin)
{
    return this->registerComponent(new MomentaryButton(name, pin, relaisPin));
}
#endif

#ifdef ARDUINOHOMELIB_USE_LIGHT_FASTLED
FastledLight* Application::makeFastledLight(const char* name, int pin, unsigned short int numLeds, CRGB* leds)
{
    return this->registerComponent(
        new FastledLight(name, pin, numLeds, leds)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_DHT
Dht22Sensor* Application::makeDht22Sensor(const char* nameTemperature, const char* nameHumidity, int pin)
{
    return this->registerComponent(
        new Dht22Sensor(nameTemperature, nameHumidity, pin)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_MAX31856
Max31856Sensor* Application::makeMax31856Sensor(const char* name, int csPin)
{
    return this->registerComponent(
        new Max31856Sensor(name, csPin)
    );
}

Max31856Sensor* Application::makeMax31856Sensor(const char* name, int csPin, int diPin, int doPin, int clkPin)
{
    return this->registerComponent(
        new Max31856Sensor(name, csPin, diPin, doPin, clkPin)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_MAX6675
Max6675Sensor* Application::makeMax6675Sensor(const char* name, int so, int cs, int clk)
{
    return this->registerComponent(
        new Max6675Sensor(name, so, cs, clk)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_SENSOR_TEMT6000
Temt6000Sensor* Application::makeTemt6000Sensor(const char* name, int pin)
{
    return this->registerComponent(
        new Temt6000Sensor(name, pin)
    );
}
#endif

#ifdef ARDUINOHOMELIB_USE_SWITCH
Switch* Application::makeSwitch(const char* name, int pin)
{
    return this->registerComponent(
        new Switch(name, pin)
    );
}
#endif

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