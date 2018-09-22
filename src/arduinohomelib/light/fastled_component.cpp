#include "arduinohomelib/light/fastled_component.h"

FastledLight::FastledLight(String name, int pin, unsigned short int numLeds, CRGB* leds) : Nameable(name)
{
    this->pin = pin;
    this->leds = leds;
    this->numLeds = numLeds;
}

void FastledLight::setup()
{
    Logger->debug("light.fastled", "Setup");
    pinMode(this->pin, OUTPUT);
    this->off();
    delay(1000);
    this->on();
    delay(5000);
    this->off();
}

void FastledLight::on()
{
    setColor(255, 0, 0);
    FastLED.show();
    Logger->debug("light.fastled", "Schalte ON");
}

void FastledLight::off()
{
    setColor(0, 0, 0);
    FastLED.show();
    Logger->debug("light.fastled", "Schalte OFF");
}

void FastledLight::setColor(int red, int green, int blue)
{
    for(unsigned short int i = 0; i < this->numLeds; i++)
    {
        this->setColorLed(i, red, green, blue);
    }
}

void FastledLight::setColorLed(unsigned short int i, int red, int green, int blue)
{
    this->leds[i].red = red;
    this->leds[i].green = green;
    this->leds[i].blue = blue;
}