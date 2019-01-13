#include <arduinohomelib.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define MQTT_SERVER "192.168.1.2"
#define MQTT_PORT 1883
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

byte mac[24] { 0xFE, 0xED, 0xBA, 0xFE, 0xFE, 0xDE };
IPAddress receiverIp(192, 168, 178, 42);

void udpMessageReceived(char* message)
{
    for (uint32_t i = 0; i < App.getComponentsCount(); i++)
    {
        App.components[i]->handleUdpMessage(atoi(message), "");
    }
}

void setup()
{
    App.setName("switch");
    App.initLog();
    App.initEthernet(mac);
    App.initMqtt(MQTT_SERVER, MQTT_USER, MQTT_PASSWORD);
    App.initUdp(udpMessageReceived, receiverIp);

    App.makeSwitch("Switch1", 34);
    App.makeSwitch("Switch2", 35);

    App.setup();
}

void loop()
{
    App.loop();
}

