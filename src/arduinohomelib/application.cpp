#include "arduinohomelib/application.h"

void Application::setup()
{
    Serial.begin(9600);

    Serial.print("Start ");
    Serial.println(Settings::name);

    this->initEthernet();

    this->ethernet->setup();

}

void Application::setName(const char* name)
{
    Settings::name = name;
}

EthernetComponent *Application::initEthernet()
{
    auto *ethernet = new EthernetComponent();
    this->ethernet = ethernet;

    return this->ethernet;
}

Application App;