#ifndef ARDUINOHOMELIB_COMPONENT_H
#define ARDUINOHOMELIB_COMPONENT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "arduinohomelib/helpers.h"
#include "arduinohomelib/settings.h"

const int ARDOINOHOMELIB_JSON_BUFFER_SIZE = 400;

class Component
{
    public:
        Component();
        void setup_();
        void loop_();
        virtual void setup();
        virtual void loop();

        void disableMqtt();
        void undiscovered();
        virtual void discover();
        virtual void subscribe();
        virtual String getCommandTopic();
        virtual void handleMqttConnected();
        virtual void handleMqttMessage(String cmd);

        virtual void handleUdpMessage(int pin, const char* cmd);

        bool cancelInterval(const std::string &name);
        void setInterval(uint32_t interval, const std::string &name = "");
        virtual void handleInterval();
        bool cancelTimeout(const std::string &name);
        void setTimeout(uint32_t timeout, const std::string &name = "");
        virtual void handleTimeout();

        void addIntervalCallback(void (*function)());
        void addTimeoutCallback(void (*function)());

    protected:
        String friendlyName;
        String fullId;

        bool useMqtt = false;
        bool isDiscovered = false;
        String commandTopic;
        String stateTopic;
        String discoveryTopic;

        CallbackManager<void()> intervalCallbacks{};
        CallbackManager<void()> timeoutCallbacks{};

        void loopInternal();

        void setDiscoveryInfo();

        /// Internal struct for storing timeout/interval functions.
        struct TimeFunction
        {
            std::string name; ///< The name/id of this TimeFunction.
            enum Type { TIMEOUT, INTERVAL, DEFER } type; ///< The type of this TimeFunction. Either TIMEOUT, INTERVAL or DEFER.
            uint32_t interval; ///< The interval/timeout of this function.
            /// The last execution for interval functions and the time, SetInterval was called, for timeout functions.
            uint32_t last_execution;
            Component *c; ///< The function (or callback) itself.
            bool remove;

            bool shouldRun(uint32_t now) const;
        };

        bool cancelTimeFunction(const std::string &name, TimeFunction::Type type);

        std::vector<TimeFunction> timeFunctions;
};

class Nameable
{
    public:
        explicit Nameable(String name);

        void setName(String name);

        String getName() const { return this->name; }
        String getId() const { return this->id; }
    protected:
        String id;
        String name;
};

#include "arduinohomelib/mqtt/mqtt_client.h"

#endif