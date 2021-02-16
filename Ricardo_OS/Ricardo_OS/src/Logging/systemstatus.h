/*
handles all messages as a bitfield so multiple messages can be contained in a single variable

*/

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Arduino.h"
#include "flags.h"
#include <string>



class SystemStatus{
    public:
        SystemStatus();
        
        void new_message(system_flag flag,std::string reason);
        void new_message(system_flag flag);
        uint32_t get_string();

        void delete_message(system_flag flag);
        bool flag_triggered(system_flag flag);
    private:
        uint32_t _status;
};

#endif