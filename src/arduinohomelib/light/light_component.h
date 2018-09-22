#ifndef ARDUINOHOMELIB_LIGHT_COMPONENT_H
#define ARDUINOHOMELIB_LIGHT_COMPONENT_H

#include "arduinohomelib/component.h"

class LightComponent : public Component
{
    public:

        void on();
        void off();

        void fadeOut();
        void fadeIn();

        void setRGB(int r, int g, int b);
        void setBrightness(int brightness);
        void setEffect(String effect);

        void handleMqttMessage(String cmd) override;

    protected:

        void handleJson();
};

#endif