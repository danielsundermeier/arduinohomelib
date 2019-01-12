#include <arduinohomelib.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define MQTT_SERVER "192.168.1.2"
#define MQTT_PORT 1883
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#define ONE_WIRE_BUS 22

byte mac[24] { 0xFE, 0xED, 0xBA, 0xFE, 0xFE, 0xAB };

OneWire oneWire(ONE_WIRE_BUS);

DeviceAddress address1 = {0x28, 0xE3, 0xFA, 0x45, 0x92, 0x0D, 0x02, 0x46};
DeviceAddress address2 = {0x28, 0x2A, 0xDA, 0x45, 0x92, 0x19, 0x02, 0x60};

void setup()
{
    App.setName("temperature");
    App.initLog();
    App.initEthernet(mac);
    App.initMqtt(MQTT_SERVER, MQTT_USER, MQTT_PASSWORD);

    DallasTemperatureSensor* dallas = App.registerComponent(new DallasTemperatureSensor(ONE_WIRE_BUS, oneWire));

    dallas->setUpdateInterval(6000);
    dallas->setValuesSendCount(10);

    dallas->add("Temp1", address1);
    dallas->add("Temp2", address2);

    App.setup();
}

void loop()
{
    App.loop();
}

