/*
handles all messages as a bitfield so multiple messages can be contained in a single variable

*/

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Arduino.h"
#include "flags.h"
#include <string>
#include "logController.h"

//class stateMachine;//forward declaration

class SystemStatus{
    public:
        //SystemStatus(stateMachine* sm);
        SystemStatus(LogController* logcontroller);
        void new_message(SYSTEM_FLAG flag,std::string info);
        void new_message(SYSTEM_FLAG flag);

        uint32_t get_string();

        void delete_message(SYSTEM_FLAG flag);
        void delete_message(SYSTEM_FLAG flag,std::string info);


        bool flag_triggered(SYSTEM_FLAG flag);
    private:
        //stateMachine* _sm;//pointer to statemachine
        LogController* _logcontroller;//pointer to logcontroller
        uint32_t _status;
};

#endif