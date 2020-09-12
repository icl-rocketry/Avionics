#include "messages.h"


//global system status variable intialized at 0
uint16_t system_status = 0;

void new_message(uint16_t msg,String reason){

    system_status = system_status + msg;


    //todo detect when in usb debug mode and print reason over serial. Maybe worth logging reason to sd card??
    


};

void delete_message(uint16_t msg){

    system_status = system_status - msg;


};
