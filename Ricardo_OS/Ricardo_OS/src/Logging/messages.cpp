#include "messages.h"


//global system status variable intialized at 0
uint32_t system_status = 0;

void new_message(uint32_t msg,String reason){

    system_status = system_status + msg;


    //todo detect when in usb debug mode and print reason over serial. Maybe worth logging reason to sd card??



};

void delete_message(uint32_t msg){

    system_status = system_status - msg;


};
