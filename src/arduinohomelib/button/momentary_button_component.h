#ifndef ARDUINOHOMELIB_MOMENTARY_BUTTON_COMPONENT_H
#define ARDUINOHOMELIB_MOMENTARY_BUTTON_COMPONENT_H

#include <Bounce2.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "arduinohomelib/network/udp_component.h"

#define DC_GAP 250
#define HOLDTIME 2000
#define LONGHOLDTIME 5000

const short int EVENT_CLICK = 1;
const short int EVENT_DOUBLECLICK = 2;
const short int EVENT_HOLD = 3;
const short int EVENT_LONGHOLD =4;

const String EVENT_TYPES[5] = {"", "click", "doubleclick", "hold", "longhold"};

class MomentaryButton
{
    public:
        const int BOUNCER_INTERVAL = 25;
        MomentaryButton();
        MomentaryButton(uint8_t pin);
        void check();
        void setPin(uint8_t pin);
        void setRelaisPin(uint8_t pin);
    private:
        String _topic;
        uint8_t _pin;
        uint8_t _relaisPin = 0;
        Bounce _bouncer;
        boolean DCwaiting = false; // whether we're waiting for a double click (down)
        boolean DConUp = false; // whether to register a double click on next release, or whether to wait and click
        boolean singleOK = true; // whether it's OK to do a single click
        long downTime = -1; // time the button was pressed down
        long upTime = -1; // time the button was released
        boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered
        boolean waitForUp = false; // when held, whether to wait for the up event
        boolean holdEventPast = false; // whether or not the hold event happened already
        boolean longHoldEventPast = false;// whether or not the long hold event happened already
        int getEvent();
        void click();
        void doubleClick();
        void hold();
        void longHold();
        void handleClick(unsigned short int eventType);
        void publish(unsigned short int eventType);
};

extern EthernetClient ethernetClient;
extern PubSubClient mqttClient;
extern IPAddress relaisControllerIp;
extern UdpComponent Udp;

#endif