#include "systemstatus.h"
#include <Arduino.h>
#include "flags.h"
#include "stateMachine.h"
#include "logController.h"

#include <string>

/*
SystemStatus::SystemStatus(stateMachine* sm):
_sm(sm),
_status(0)
{};*/


SystemStatus::SystemStatus(LogController* logcontroller):
_logcontroller(logcontroller),
_status(0)
{};

void SystemStatus::new_message(system_flag flag,std::string info){

    _status |= static_cast<uint32_t>(flag);
    //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),info);
    _logcontroller->log(_status,static_cast<uint32_t>(flag),info);

};

uint32_t SystemStatus::get_string(){
    return _status;
}

void SystemStatus::new_message(system_flag flag){
    _status |= static_cast<uint32_t>(flag);
    //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),"flag raised");
    _logcontroller->log(_status,static_cast<uint32_t>(flag),"flag raised");
};


void SystemStatus::delete_message(system_flag flag){
    _status &= ~static_cast<uint32_t>(flag);
   // _sm->logcontroller.log(_status,static_cast<uint32_t>(flag),"flag removed");
    _logcontroller->log(_status,static_cast<uint32_t>(flag),"flag removed");
};

void SystemStatus::delete_message(system_flag flag,std::string info){

    _status &= ~static_cast<uint32_t>(flag);
   //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),info);
    _logcontroller->log(_status,static_cast<uint32_t>(flag),info);
};


bool SystemStatus::flag_triggered(system_flag flag){
    return (_status & static_cast<uint32_t>(flag));
};