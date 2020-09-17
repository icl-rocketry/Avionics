#include <Arduino.h>

#include "ricardo_pins.h"


#include "Comms/webserver.h"
#include "Sensors/battery.h"




//get core id using xPortGetCoreID()


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  #ifdef VERBOSE
    Serial.println("Displaying all debug messages...");
  #endif


  setupWebserver();
  

}

void loop() {
  // put your main code here, to run repeatedly:
}