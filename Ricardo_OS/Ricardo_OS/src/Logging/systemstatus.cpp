#include "systemstatus.h"
#include <Arduino.h>
#include "flags.h"

#include <string>


SystemStatus::SystemStatus():
_status(0)
{};

void SystemStatus::new_message(system_flag flag,std::string reason){

    _status |= static_cast<uint32_t>(flag);
    //detect debug mode
    if (flag_triggered(system_flag::STATE_USBMODE)){
        //Serial print reason and log it
       Serial.println(reason.c_str());
    };

};

uint32_t SystemStatus::get_string(){
    return _status;
}

void SystemStatus::new_message(system_flag flag){
    _status |= static_cast<uint32_t>(flag);
};


void SystemStatus::delete_message(system_flag flag){
    _status &= ~static_cast<uint32_t>(flag);
};

bool SystemStatus::flag_triggered(system_flag flag){
    return (_status & static_cast<uint32_t>(flag));
};