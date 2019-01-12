#include <arduinohomelib.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define MQTT_SERVER "192.168.1.2"
#define MQTT_PORT 1883
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#define TRIGGER_PIN 9
#define ECHO_PIN 8

byte mac[24] { 0xFE, 0xED, 0xBA, 0xFE, 0xFE, 0xAC };

void setup()
{
    App.setName("distance_controller");
    App.initLog();
    App.initEthernet(mac);
    App.initMqtt(MQTT_SERVER, MQTT_USER, MQTT_PASSWORD);

    App.registerComponent(new Hcsr04Sensor("Distance", TRIGGER_PIN, ECHO_PIN));

    App.setup();
}

void loop()
{
    App.loop();
}