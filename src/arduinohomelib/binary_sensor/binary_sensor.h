#ifndef ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H

#include <vector>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class BinarySensor : public Component, public Nameable
{
    public:
        BinarySensor(String name, int pin);

        void setup();
        void loop();

        bool toHigh();
        bool toLow();

        void discover();
        void handleMqttConnected();
        void sendState();

        void addOnNewStateCallback(void (*function)(bool state));
        void callOnNewStateCallback(bool state);

    protected:
        const char* device = "binary_sensor";

        std::vector<void (*)(bool)> onNewStateCallbacks;

        int pin;

        bool lastState = false;
        bool state = false;

        void setDiscoveryInfo();
        virtual const char* getDeviceClass() const;
};

#endif