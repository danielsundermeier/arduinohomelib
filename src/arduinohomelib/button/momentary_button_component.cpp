#include "arduinohomelib/button/momentary_button_component.h"

MomentaryButton::MomentaryButton() {}

MomentaryButton::MomentaryButton(uint8_t pin)
{
    setPin(pin);
}

void MomentaryButton::setPin(uint8_t pin)
{
    _bouncer = Bounce();
    _bouncer.attach(pin, INPUT_PULLUP);
    _bouncer.interval(BOUNCER_INTERVAL);
    _pin = pin;
    _topic = "switch-controller-1/" + String(_pin) + "/";
}

void MomentaryButton::setRelaisPin(uint8_t pin)
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
    Serial.print("Momentary Button ");
    Serial.print(_pin);
    Serial.println(" setup");
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
        Serial.println(String(receiverIp[0]) + "." + String(receiverIp[1]) + "." + String(receiverIp[2]) + "." + String(receiverIp[3]));
        globalUdpComponent->send(String(_relaisPin));
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
    Serial.println("Button " + String(_pin) + " " + EVENT_TYPES[eventType]);
    publish(eventType);
}

void MomentaryButton::publish(unsigned short int eventType)
{
    String topic = _topic + EVENT_TYPES[eventType];
    Serial.println(topic);
    globalMqttClient->publish(topic.c_str(), "ON");
}