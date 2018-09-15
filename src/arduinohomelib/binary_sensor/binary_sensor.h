#ifndef ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H

#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class BinarySensor : public Component
{
    public:
        BinarySensor(int pin);

        void setup();
        void loop();

        bool toHigh();
        bool toLow();

        void discover();
        void handleMqttConnected();
        void sendState();

    protected:
        const char* device = "binary_sensor";

        int pin;

        bool lastState = false;
        bool state = false;

        void setDiscoveryInfo();
        virtual const char* getDeviceClass() const;
};

#endif