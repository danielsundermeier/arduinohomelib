#ifndef ARDUINOHOMELIB_HELPERS_H
#define ARDUINOHOMELIB_HELPERS_H

#include <Arduino.h>
#include <vector>

String toKebabCase(String text);

template<typename... X> class CallbackManager;

template<typename... Ts>
class CallbackManager<void(Ts...)> {
    public:

        void add(void (*function)(Ts...));

        void call(Ts... args);

    protected:
        std::vector<void (*)(Ts...)> callbacks;
};

template<typename... Ts>
void CallbackManager<void(Ts...)>::add(void (*function)(Ts...))
{
    this->callbacks.push_back(function);
}

template<typename... Ts>
void CallbackManager<void(Ts...)>::call(Ts... args)
{
    for (uint32_t i = 0; i < this->callbacks.size(); i++)
    {
        this->callbacks[i](args...);
    }
}

#endif