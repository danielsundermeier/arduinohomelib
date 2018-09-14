#include "arduinohomelib/log_component.h"

LogComponent::LogComponent()
{
    Serial.begin(9600);

    Logger = this;
}

void LogComponent::setup()
{

}

int LogComponent::debug(const char *tag, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    int ret = log_sprinf(ARDUINOHOMELIB_LOG_LEVEL_DEBUG, tag, format, arg);
    va_end(arg);

    return ret;
}

int LogComponent::log_sprinf(int level, const char *tag, const char *format, va_list args)
{
    int ret = vsnprintf(buffer, ARDUINOHOMELIB_LOG_BUFFER_SIZE, format, args);

    String message = String(tag) + "\t"+ String(buffer);
    Serial.println(message.c_str());
    // globalMqttClient->log(message.c_str());

    return ret;
}

LogComponent *Logger = nullptr;