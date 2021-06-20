#include <Arduino.h>

// #include "config.h"

// #include "stateMachine.h"
// #include "States/setup.h"
// #include "Comms/packets.h"
// #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
// #include "esp_log.h"




// stateMachine statemachine;


// void setup() {
//   esp_log_level_set("*", ESP_LOG_INFO);        // set all components to ERROR level
//   statemachine.initialise(new Setup(&statemachine)); //intialize statemachine with setup state to run all necessary setup tasks.
// }

// void loop() {
//   statemachine.update();

  
// }

#include <vector>


struct entry{
  int a;
  int b;
};

std::vector< std::vector<entry> > _table(2,std::vector<entry>(5));


void seteup(){
  
  Serial.begin(115200);
  _table[0] = std::vector<entry>{{1,1},{2,1},{3,1},{4,1},{5,1}};
  _table[1] = std::vector<entry>{{2,1},{3,1},{4,1},{5,1},{6,1}};

  for (int i = 0; i<2 ; i++){
        for (int j = 0; j<5; j++){
            Serial.print(_table.at(i).at(j).a);
            Serial.print(",");
            
        }
        Serial.println();
    }


}
void loop(){

}