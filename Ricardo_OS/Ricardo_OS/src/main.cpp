#include <Arduino.h>

#include "config.h"

#include "stateMachine.h"
#include "States/setup.h"
#include "Comms/packets.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"




stateMachine statemachine;


void setup() {
  esp_log_level_set("*", ESP_LOG_INFO);        // set all components to ERROR level
  statemachine.initialise(new Setup(&statemachine)); //intialize statemachine with setup state to run all necessary setup tasks.
}

void loop() {
  statemachine.update();

  
}
