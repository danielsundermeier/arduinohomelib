#ifndef ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_BINARY_SENSOR_COMPONENT_H

#include <vector>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class BinarySensor : public Component, public Nameable
{
    public:
        BinarySensor(const char* name, int pin);

        void setup();
        void loop();

        bool toHigh();
        bool toLow();

        void discover();
        void handleMqttConnected();
        void sendState();

        void addStateCallback(void (*function)(bool state));

    protected:
        const char* device = "binary_sensor";

        CallbackManager<void(bool)> stateCallback{};

        int pin;

        bool lastState = false;
        bool state = false;

        virtual const char* getDeviceClass() const { return "None"; }
        const char* getDevice() const { return "binary_sensor"; }
};

#endif