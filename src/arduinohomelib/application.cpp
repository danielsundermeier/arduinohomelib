#include "arduinohomelib/application.h"

EthernetClient ethClient;
PubSubClient pubSubClient(ethClient);

void Application::setup()
{
    Serial.begin(9600);

    Serial.print("Start ");
    Serial.println(Settings::name);
}

void Application::loop()
{

}

void Application::setName(const char* name)
{
    Settings::name = name;
}

EthernetComponent *Application::initEthernet()
{
    auto *ethernet = new EthernetComponent();
    this->ethernet = ethernet;

    return this->ethernet;
}

MqttClient *Application::initMqtt(String serverId, String username, String password)
{
    struct MQTTCredentials credentials { serverId, 1883, username, password, Settings::name };
    auto *mqtt = new MqttClient(pubSubClient, credentials);
    this->mqtt = mqtt;

    return this->mqtt;
}

UdpComponent *Application::initUdp(IPAddress receiverIp)
{
    auto *udp = new UdpComponent(receiverIp);
    this->udp = udp;

    return this->udp;
}

Application App;