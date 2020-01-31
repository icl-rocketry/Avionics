#include <Arduino.h>

#include "config.h"
#include "WebServer.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  #ifdef DEBUG
    Serial.println("Displaying all debug messages...");
  #endif

  setupWebserver();
  

}

void loop() {
  // put your main code here, to run repeatedly:
}
