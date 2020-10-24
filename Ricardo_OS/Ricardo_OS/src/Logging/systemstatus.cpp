#include "systemstatus.h"
#include <Arduino.h>
#include "flags.h"


SystemStatus::SystemStatus(){
    _status = 0; //initialize status to 0
};

void SystemStatus::new_message(system_flag flag,String reason){

    _status |= static_cast<uint32_t>(flag);

    //detect debug mode
    if (flag_triggered(system_flag::STATE_USBMODE)){
        //Serial print reason and log it
       Serial.println(reason);
    };

};

void SystemStatus::new_message(system_flag flag){
    _status |= static_cast<uint32_t>(flag);
};


void SystemStatus::delete_message(system_flag flag){
    _status &= ~static_cast<uint32_t>(flag);
};

bool SystemStatus::flag_triggered(system_flag flag){
    return (_status & static_cast<uint32_t>(flag));
};