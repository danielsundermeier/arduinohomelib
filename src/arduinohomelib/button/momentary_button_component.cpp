#include "arduinohomelib/button/momentary_button_component.h"

MomentaryButton::MomentaryButton() {}

MomentaryButton::MomentaryButton(int pin)
{
    setPin(pin);
}

MomentaryButton::MomentaryButton(int pin, int relaisPin)
{
    setPin(pin);
    setRelaisPin(relaisPin);
}

void MomentaryButton::setPin(int pin)
{
    _bouncer = Bounce();
    _bouncer.attach(pin, INPUT_PULLUP);
    _bouncer.interval(BOUNCER_INTERVAL);
    _pin = pin;
    _topic = String(Settings::name) + "/" + String(_pin) + "/";
}

void MomentaryButton::setRelaisPin(int pin)
{
    _relaisPin = pin;
}

int MomentaryButton::getEvent()
{
    int event = 0;
    _bouncer.update();
    int buttonVal = _bouncer.read();
    if (_bouncer.fell())
    {
        downTime = millis();
        ignoreUp = false;
        waitForUp = false;
        singleOK = true;
        holdEventPast = false;
        longHoldEventPast = false;
        if ((millis()-upTime) < DC_GAP && DConUp == false && DCwaiting == true)
        {
            DConUp = true;
        }
        else
        {
            DConUp = false;
        }
        DCwaiting = false;
    }
    else if(_bouncer.rose())
    {
        if (not ignoreUp)
        {
            upTime = millis();
            if (DConUp == false)
            {
                DCwaiting = true;
            }
            else
            {
                event = EVENT_DOUBLECLICK;
                DConUp = false;
                DCwaiting = false;
                singleOK = false;
            }
        }
    }

    if (buttonVal == HIGH && (millis()-upTime) >= DC_GAP && DCwaiting == true && DConUp == false && singleOK == true)
    {
        event = EVENT_CLICK;
        DCwaiting = false;
    }

    if (buttonVal == LOW && (millis() - downTime) >= HOLDTIME)
    {
        // Trigger "normal" hold
        if (not holdEventPast)
        {
            event = EVENT_HOLD;
            waitForUp = true;
            ignoreUp = true;
            DConUp = false;
            DCwaiting = false;
            //downTime = millis();
            holdEventPast = true;
        }
        // Trigger "long" hold
        if ((millis() - downTime) >= LONGHOLDTIME)
        {
            if (not longHoldEventPast) {
                event = EVENT_LONGHOLD;
                longHoldEventPast = true;
            }
        }
    }

    return event;
}

void MomentaryButton::setup()
{
    Logger->debug("button.momentary", "Setup %2d", _pin);

    // this->setInterval("interval", 2000, []() { Serial.println("Interval Function"); });
    // this->setTimeout("timeout", 3000, []() { Serial.println("Timeout Function"); });
}

void MomentaryButton::loop()
{
    switch (getEvent()) {
        case EVENT_CLICK: click(); break;
        case EVENT_DOUBLECLICK: doubleClick(); break;
        case EVENT_HOLD: hold(); break;
        case EVENT_LONGHOLD: longHold(); break;
    }
}

void MomentaryButton::click()
{
    handleClick(EVENT_CLICK);
    if (_relaisPin > 0)
    {
        globalUdpComponent->sendCommand(_relaisPin, "toggle");
    }
}

void MomentaryButton::doubleClick()
{
    handleClick(EVENT_DOUBLECLICK);
}

void MomentaryButton::hold()
{
    handleClick(EVENT_HOLD);
}

void MomentaryButton::longHold()
{
    handleClick(EVENT_LONGHOLD);
}

void MomentaryButton::handleClick(unsigned short int eventType)
{
    Logger->debug("button.momentary", "Button %2d\t%s", _pin, EVENT_TYPES[eventType].c_str());
    publish(eventType);
}

void MomentaryButton::publish(unsigned short int eventType)
{
    String topic = _topic + EVENT_TYPES[eventType];
    globalMqttClient->publish(topic.c_str(), "ON");
}