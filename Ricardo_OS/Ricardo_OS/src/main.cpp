#include <Arduino.h>
#include "stateMachine.h"
#include "States/setup.h"


stateMachine statemach;

void setup() {
  statemach.initialise(new Setup(&statemach)); //intialize statemachine with setup state to run all necessary setup tasks.

}

void loop() {
  statemach.update();
  
}
