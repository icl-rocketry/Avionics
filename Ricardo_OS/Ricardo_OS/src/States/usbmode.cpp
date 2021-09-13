#include "Arduino.h"
#include "usbmode.h"

#include "stateMachine.h"

#include "flags.h"

USBmode::USBmode(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_USBMODE)
{};

void USBmode::initialise(){
    State::initialise();
};

State* USBmode::update(){
    
    return this;
};

void USBmode::exitstate(){
    State::exitstate();
};