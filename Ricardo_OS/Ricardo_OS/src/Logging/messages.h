/*
handles all messages bitwise so multiple messages can be contained in a single variable


//reqwrite as class
*/

#ifndef MESSAGES_H
#define MESSAGES_H

#include "Arduino.h"



void new_message(uint32_t msg,String reason);
void delete_message(uint32_t msg);
bool flag_triggered(uint32_t msg);


class SystemStatus{
    public:
        void new_message(uint32_t )
}
#endif