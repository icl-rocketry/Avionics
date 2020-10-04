#include "Arduino.h"
#include "usbmode.h"

#include "stateMachine.h"

USBmode::USBmode(stateMachine* sm) : State(sm){
    curr_stateID = 10; // random id

}

void USBmode::initialise(){
    State::initialise();
}

State* USBmode::update(){
    //if serial command recived transtion to groundstation mode
    return this;
}

void USBmode::exitstate(){
    State::exitstate();
};