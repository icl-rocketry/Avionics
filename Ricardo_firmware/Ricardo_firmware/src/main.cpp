#include <Arduino.h>

#include "config.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  #ifdef DEBUG
    Serial.println("showing all debug messages...");
  #endif

}

void loop() {
  // put your main code here, to run repeatedly:
}
