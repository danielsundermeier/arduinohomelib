# arduinohomelib

**arduinohomelib** is a library designed to greatly simplify your firmware code for Arduino devices with full seamless [Home Assistant](https://www.home-assistant.io/) integration (with automatic MQTT discovery!) so that you can focus on creating the hardware, not the software.

inspired by [esphomelib](https://github.com/OttoWinter/esphomelib)

## Example
```cpp
#include <arduinohomelib.h>

#define MQTT_SERVER "192.168.1.2"
#define MQTT_USER "USERNAME"
#define MQTT_PASSWORD "PASSWORD"

#define MAXX6675_SCK 2
#define MAX6675_CS 3
#define MAX6675_SO 4
#define DHT22_PIN 5
#define WS2813_PIN 7
#define MOTION_PIN 8
#define BUTTON_PIN 9
#define SWITCH_PIN 13
#define TEMT6000_PIN A0

#define NUM_LEDS 5

byte mac[24] { 0xFE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress receiverIp(192, 168, 1, 3);
MomentaryButton* button12;
FastledLight* ws2813;
CRGB leds[NUM_LEDS];

Switch* led;

void setup()
{
    App.setName("testduino");
    App.initLog();

    App.initEthernet(mac);
    App.initMqtt(MQTT_SERVER, MQTT_USER, MQTT_PASSWORD, messageReceived, mqttConnected);
    App.initUdp(udpMessageReceived, receiverIp);

    Temt6000Sensor* temt6000 = App.makeTemt6000Sensor("Brightness Livingroom", TEMT6000_PIN);
    temt6000->setUpdateInterval(6000);
    temt6000->setValuesSendCount(10);

    App.makeMax6675Sensor("temperature", MAX6675_SO, MAX6675_CS, MAXX6675_SCK);

    MomentaryButton* button10 = App.makeMomentaryButton("Button Livingroom", BUTTON_PIN);
    // ON click: MQTT Message + UDP Message 1:toggle to receiverIp
    // ON doubleclick, hold, longhold MQTT Message
    button10->setRelaisPin(1);

    led = App.makeSwitch("LED Livingroom", SWITCH_PIN);
    led->addTimeoutCallback(ledOff);

    Hcsr501BinarySensor* hcsr501 = App.makeHcsr501BinarySensor("Motion Livingroom", MOTION_PIN);
    // LED off after 10s
    hcsr501->addStateCallback(onStateChange);

    // WIP, no MQTT yet
    FastLED.addLeds<WS2812B, WS2813_PIN, GRB>(leds, NUM_LEDS);
    ws2813 = App.makeFastledLight("LED Stripe", WS2813_PIN, NUM_LEDS, leds);

    Dht22Sensor* dht = App.makeDht22Sensor("Temperature Livingroom", "Humidity Livingroom", DHT22_PIN);
    // Avarage Value to MQTT after 10 Values (every 6s*10 = 60s)
    dht->setValuesSendCount(10);
    // New value every 6s
    dht->setUpdateInterval(6000);

    App.setup();
}

void loop()
{
    App.loop();
}

void onStateChange(bool state)
{
    if (! state)
    {
        return;
    }
    led->on();
    led->setTimeout(10000, "ledOff");
}

void ledOff()
{
    led->off();
}

void messageReceived(char* topic, byte* payload, unsigned int length) {
    App.handleMqttMessage(topic, payload, length);
}

void udpMessageReceived(char* message)
{
    App.udp->responde();
}

void mqttConnected()
{
    for (uint32_t i = 0; i < App.getComponentsCount(); i++) {
        App.components[i]->handleMqttConnected();
    }
}
```

## Installation

1. Download the folder (https://github.com/OttoWinter/esphomelib/releases)
2.  (In the Arduino IDE)  **Sketch > Include Library > Add .ZIP Library... >**  select the downloaded file  **> Open**
