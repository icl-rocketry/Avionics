#include <Arduino.h>
#include "ricardo_pins.h"
#include "config.h"



#include "stateMachine.h"
#include "States/setup.h"


stateMachine statemach;


//get core id using xPortGetCoreID()


void setup() {

  //currently have to set packet handler here 
  statemach.downlink.usbserial._ps.setPacketHandler([](const uint8_t* buffer, size_t size){
    statemach.downlink.usbserial.onPacketReceived(buffer,size);
  });

  #ifdef VERBOSE
    Serial.println("Displaying all debug messages...");
  #endif

  statemach.initialise(new Setup(&statemach)); //intialize statemachine with setup state to run all necessary setup tasks.
}

void loop() {
  // put your main code here, to run repeatedly:
  statemach.update();
}
