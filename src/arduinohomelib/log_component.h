#ifndef ARDUINOHOMELIB_LOG_COMPONENT_H
#define ARDUINOHOMELIB_LOG_COMPONENT_H

#include "arduinohomelib/component.h"

#define ARDUINOHOMELIB_LOG_LEVEL_NONE 0
#define ARDUINOHOMELIB_LOG_LEVEL_ERROR 1
#define ARDUINOHOMELIB_LOG_LEVEL_WARN 2
#define ARDUINOHOMELIB_LOG_LEVEL_INFO 3
#define ARDUINOHOMELIB_LOG_LEVEL_DEBUG 4
#define ARDUINOHOMELIB_LOG_LEVEL_VERBOSE 5
#define ARDUINOHOMELIB_LOG_LEVEL_VERY_VERBOSE 6

#define ARDUINOHOMELIB_LOG_BUFFER_SIZE 512

class LogComponent : public Component
{
    public:
        LogComponent();
        int debug(const char *tag, const char *format, ...);
        void setup() override;
    protected:
        char buffer [ARDUINOHOMELIB_LOG_BUFFER_SIZE];
        int log_sprinf(int level, const char *tag, const char *format, va_list args);
};

extern LogComponent *Logger;

#endif