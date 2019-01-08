#include "arduinohomelib/switch/i2c_switch_component.h"

I2CSwitch::I2CSwitch(const char* name, int pin, Adafruit_MCP23017 mcp) : Nameable(name)
{
    setPin(pin);
    this->mcp = mcp;
}

void I2CSwitch::setPin(int pin)
{
    this->pin = pin;
    this->mcp.pinMode(this->pin, OUTPUT);
}

void I2CSwitch::on()
{
    write(HIGH);
}

void I2CSwitch::off()
{
    write(LOW);
}

void I2CSwitch::toggle()
{
    write(! read());
}

void I2CSwitch::write(int value)
{
    Logger->debug("I2CSwitch", "Schalte\t[%2d]\t%d", this->pin, value);

    this->mcp.digitalWrite(this->pin, value);

    if (this->useMqtt == false)
    {
        return;
    }

    globalMqttClient->publish(this->getTopic("state"), (read() == 1 ? "ON" : "OFF"));
}

int I2CSwitch::read()
{
    return this->mcp.digitalRead(this->pin);
}

void I2CSwitch::subscribe()
{
    globalMqttClient->subscribe(this->getTopic("set"));
}

void I2CSwitch::handleMqttConnected()
{
    if (this->useMqtt == false)
    {
        return;
    }

    this->subscribe();
    if (isDiscovered == false)
    {
        discover();
    }
}

void I2CSwitch::handleMqttMessage(String cmd)
{
    if (this->useMqtt == false)
    {
        return;
    }

    if (strcmp(cmd.c_str(), "ON") == 0)
    {
        on();
    }
    else if (strcmp(cmd.c_str(), "OFF") == 0)
    {
        off();
    }
    else if (strcmp(cmd.c_str(), "TOGGLE") == 0)
    {
        toggle();
    }
}

void I2CSwitch::handleUdpMessage(int pin, const char* cmd)
{
    if (pin == this->pin)
    {
        if (strcmp(cmd, "") == 0)
        {
            this->toggle();
        }
        else if (strcmp(cmd, "on") == 0)
        {
            this->on();
        }
        else if (strcmp(cmd, "off") == 0)
        {
            this->off();
        }
        else if (strcmp(cmd, "toggle") == 0)
        {
            this->toggle();
        }
    }
}

void I2CSwitch::discover()
{
    StaticJsonBuffer<ARDOINOHOMELIB_JSON_BUFFER_SIZE> JSONbuffer;
    JsonObject& discoveryInfo = JSONbuffer.createObject();

    discoveryInfo["platform"] = "mqtt";
    discoveryInfo["name"] = this->getName();
    discoveryInfo["state_topic"] = this->getTopic("state");
    discoveryInfo["command_topic"] = this->getTopic("set");
    discoveryInfo["availability_topic"] = String(Settings::name) + "/status";

    if (globalMqttClient->publish(this->getDiscoveryTopic(), discoveryInfo) == true)
    {
        globalMqttClient->publish(this->getTopic("set"), "OFF");
        isDiscovered = true;
    }
    else
    {
        Logger->debug("I2CSwitch", "Error sending discovery");
        Logger->debug("I2CSwitch", "size message: %d", discoveryInfo.measureLength() + 1);
    }
}