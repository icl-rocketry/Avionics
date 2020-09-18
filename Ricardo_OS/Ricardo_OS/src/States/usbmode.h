/*
State is called when usb is detected. No child state is called and state allows any state to be entered and exited for debugging purposes.
FLash can be dumped over the serial terminal and extra commands avaliabe over serial interface to improve debugging.
*/



#ifndef USBMODE_H
#define USBMODE_H
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

class USBmode: public State {
  public:
    void initialise();
    State* update();
};

#endif