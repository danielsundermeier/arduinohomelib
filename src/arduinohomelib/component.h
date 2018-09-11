#ifndef ARDUINOHOMELIB_COMPONENT_H
#define ARDUINOHOMELIB_COMPONENT_H

class Component
{
    public:
        Component();
        virtual void setup();
        virtual void loop();
};

#endif