#include <Arduino.h>

#include "stateMachine.h"
#include "States/setup.h"
#include "Comms/packets.h"


stateMachine statemachine;

void setup() {
  statemachine.initialise(new Setup(&statemachine)); //intialize statemachine with setup state to run all necessary setup tasks.
}
void loop() {
  statemachine.update();
  
}
