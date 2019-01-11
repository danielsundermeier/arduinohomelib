#ifndef ARDUINOHOMELIB_SENSOR_COMPONENT_H
#define ARDUINOHOMELIB_SENSOR_COMPONENT_H

#include <algorithm>
#include <queue>
#include <MemoryFree.h>
#include "arduinohomelib/component.h"
#include "arduinohomelib/log_component.h"
#include "arduinohomelib/mqtt/mqtt_client.h"

class SensorComponent : public Component, public Nameable
{
    public:
        SensorComponent(const char* name);

        virtual void update();

        void handleInterval() override;

        void discover() override;
        void handleMqttConnected();
        virtual void sendValue();

        bool shouldSendValue();
        void resetValuesCount();

        void setDeviceClass(String deviceClass);
        void setIcon(String icon);
        void setUnitOfMeassurement(String unitOfMeassurement);
        void setAccuracyDecimals(short unsigned int accuracyDecimals);

        virtual void setUpdateInterval(unsigned int updateInterval);
        virtual void setValuesSendCount(unsigned int valuesSendCount);

        void addNewValueCallback(void (*function)(double value));

    protected:
        int pin;

        String deviceClass = "";
        String icon = "";
        String unitOfMeassurement = "";
        short unsigned int accuracyDecimals = 2;

        char valueStr[10];
        char rawValueStr[10];
        double value = 0;
        double rawValue = 0;

        // Number of new Values until sendState()
        unsigned int valuesSendCount = 5;
        // Number of new Values
        unsigned int valuesCount = 0;

        unsigned int valuesMaxSize = 10;
        std::queue<double> values;
        double sum = 0;

        unsigned int updateInterval = 2000;

        CallbackManager<void(double)> newValueCallback{};

        void newRawValue(double rawValue);
        double calculateAverage();

        void setValueStr();
        void setRawValueStr();

        virtual unsigned int getUpdateInterval() const { return this->updateInterval; }
        virtual unsigned int getValuesSendCount() const { return this->valuesSendCount; }
        virtual short unsigned int getAccuracyDecimals() const { return this->accuracyDecimals; }
        virtual String getDeviceClass() const { return this->deviceClass; }
        virtual String getIcon() const { return this->icon; }
        virtual String getUnitOfMeassurement() const { return this->unitOfMeassurement; }

        const char* getDevice() const { return "sensor"; }
};

class EmptySensorComponent : public SensorComponent
{
    public:
        explicit EmptySensorComponent(const char* name, short unsigned int accuracyDecimals, String icon, String unitOfMeassurement);

        void setup();

        void setNewRawValue(double rawValue);

        short unsigned int getAccuracyDecimals() const override { return this->accuracyDecimals; }
        String getIcon() const override { return this->icon; }
        String getUnitOfMeassurement() const override { return this->unitOfMeassurement; }

        String getRawValueStr() const { return this->rawValueStr; }
        String getValueStr() const { return this->valueStr; }
    protected:
        char* getTopic(const char* suffix);
};

#endif