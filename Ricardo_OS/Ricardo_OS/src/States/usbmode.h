

#ifndef USBMODE_H
#define USBMODE_H
#include "Arduino.h"
#include "state.h"

class USBmode: public State {
  public:
    USBmode(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

#endif