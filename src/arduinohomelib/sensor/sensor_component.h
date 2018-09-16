#ifndef ARDUINOHOMELIB_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_SENSOR_COMPONENT_H

#include <algorithm>
#include <queue>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class SensorComponent : public Component
{
    public:
        SensorComponent();

        virtual void update();

        void handleInterval() override;

        void discover();
        void handleMqttConnected();
        void sendValue();

        // TODO
        // bool shouldSend() ?

        void setUpdateInterval(int updateInterval);
        void setDeviceClass(String deviceClass);

    protected:
        const char* device = "sensor";
        String deviceClass;

        int pin;

        char valueStr[10];
        double value;
        double rawValue;

        // Number of new Values until sendState()
        unsigned int valuesSendCount = 5;
        // Number of new Values
        unsigned int valuesCount = 0;

        unsigned int valuesMaxSize = 10;
        std::queue<double> values;
        double sum = 0;

        int updateInterval = 2000;

        void newRawValue(double rawValue);
        double calculateAverage();

        virtual int getUpdateInterval() const;
        virtual String getDeviceClass() const;

        void setDiscoveryInfo();
};

#endif