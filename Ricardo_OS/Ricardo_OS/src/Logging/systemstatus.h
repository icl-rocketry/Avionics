/*
handles all messages as a bitfield so multiple messages can be contained in a single variable

*/

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Arduino.h"
#include "flags.h"
#include <string>

class stateMachine;//forward declaration

class SystemStatus{
    public:
        SystemStatus(stateMachine* sm);
        
        void new_message(system_flag flag,std::string info);
        void new_message(system_flag flag);

        uint32_t get_string();

        void delete_message(system_flag flag);
        void delete_message(system_flag flag,std::string info);


        bool flag_triggered(system_flag flag);
    private:
        stateMachine* _sm;//pointer to statemachine
        uint32_t _status;
};

#endif