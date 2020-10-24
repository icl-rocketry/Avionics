/*
handles all messages bitwise so multiple messages can be contained in a single variable


//reqwrite as class
*/

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Arduino.h"
#include "flags.h"



class SystemStatus{
    public:
        SystemStatus();
        void new_message(system_flag flag,String reason);
        void delete_message(system_flag flag);
        bool flag_triggered(system_flag flag);
    protected:
        uint32_t _status;
};

#endif