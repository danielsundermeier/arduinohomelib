#ifndef ARDUINOHOMELIB_FASTLED_LIGHT_COMPONENT_H
#define ARDUINOHOMELIB_FASTLED_LIGHT_COMPONENT_H

#include "arduinohomelib/defines.h"
#ifdef ARDUINOHOMELIB_USE_LIGHT_FASTLED

#include <FastLED.h>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"

class FastledLight : public Component, public Nameable
{
    public:
        FastledLight(const char* name, int pin, unsigned short int numLeds, CRGB* leds);

        CRGB* leds;
        unsigned short int numLeds;

        void setup() override;

        void on();
        void off();
        void toggle();

        void setColor(int red, int green, int blue);
        void setColorLed(unsigned short int i, int red, int green, int blue);

    protected:

        bool state = false;
        uint8_t pin;
};

#endif

#endif