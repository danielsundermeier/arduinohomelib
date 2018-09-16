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

#include "arduinohomelib/settings.h"

const int ARDOINOHOMELIB_JSON_BUFFER_SIZE = 300;

class Component
{
    public:
        Component();
        void setup_();
        void loop_();
        virtual void setup();
        virtual void loop();

        virtual void subscribe();
        virtual String getCommandTopic();
        virtual void handleMqttConnected();
        virtual void handleMqttMessage(String cmd);

        bool cancelInterval(const std::string &name);
        void setInterval(const std::string &name, uint32_t interval, Component *c);
        virtual void handleInterval();
        //bool cancelTimeout(const std::string &name);
        //void setTimeout(const std::string &name, uint32_t timeout, void (*f)());
        virtual void handleTimeout();

    protected:
        String friendlyName;
        String id;
        String fullId;

        bool isDiscovered = false;
        String commandTopic;
        String stateTopic;
        String discoveryTopic;

        StaticJsonBuffer<ARDOINOHOMELIB_JSON_BUFFER_SIZE> JSONbuffer;
        JsonObject& discoveryInfo = JSONbuffer.createObject();

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

#endif