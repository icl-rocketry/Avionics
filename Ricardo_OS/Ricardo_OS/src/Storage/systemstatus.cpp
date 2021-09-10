#include "systemstatus.h"
#include "flags.h"
#include "stateMachine.h"
#include "logController.h"

#include <string>


SystemStatus::SystemStatus(LogController* logcontroller):
_logcontroller(logcontroller),
_status(0)
{};

void SystemStatus::new_message(SYSTEM_FLAG flag,std::string info){

    _status |= static_cast<uint32_t>(flag);
    //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),info);
    _logcontroller->log(_status,static_cast<uint32_t>(flag),info);

};

uint32_t SystemStatus::getStatus(){
    return _status;
}

void SystemStatus::new_message(SYSTEM_FLAG flag){
    _status |= static_cast<uint32_t>(flag);
    //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),"flag raised");
    _logcontroller->log(_status,static_cast<uint32_t>(flag),"flag raised");
};


void SystemStatus::delete_message(SYSTEM_FLAG flag){
    _status &= ~static_cast<uint32_t>(flag);
   // _sm->logcontroller.log(_status,static_cast<uint32_t>(flag),"flag removed");
    _logcontroller->log(_status,static_cast<uint32_t>(flag),"flag removed");
};

void SystemStatus::delete_message(SYSTEM_FLAG flag,std::string info){

    _status &= ~static_cast<uint32_t>(flag);
   //_sm->logcontroller.log(_status,static_cast<uint32_t>(flag),info);
    _logcontroller->log(_status,static_cast<uint32_t>(flag),info);
};


