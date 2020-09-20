#include <Arduino.h>
#include "ricardo_pins.h"
#include "config.h"
#include "internal_io.h"
#include "Logging/messages.h"


#include "stateMachine.h"
#include "States/preflight.h"


stateMachine statemach;


//get core id using xPortGetCoreID()


void setup() {
  setup_io();

  #ifdef VERBOSE
    Serial.println("Displaying all debug messages...");
  #endif

  statemach.initialise(new Preflight(&statemach));
}

void loop() {
  // put your main code here, to run repeatedly:
  statemach.update();
}
