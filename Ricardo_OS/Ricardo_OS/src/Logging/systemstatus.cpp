#include "systemstatus.h"
#include <Arduino.h>



SystemStatus::SystemStatus(){
    _status = 0; //initialize status to 0
}

void SystemStatus::new_message(system_flag flag,String reason){

    _status |= flag;

    //detect debug mode
    if (flag_triggered(system_flag::STATE_USBMODE)){
        //Serial print reason and log it
       Serial.println(reason);
    };

};

void SystemStatus::delete_message(system_flag flag){
    _status &= ~flag;
};

bool SystemStatus::flag_triggered(system_flag flag){
    return (_status & flag);
};