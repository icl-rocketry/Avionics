#include "messages.h"
#include "config.h"

//global system status variable intialized at 0
uint32_t system_status = 0;

void new_message(uint32_t msg,String reason){

    system_status |= msg;

    //detect debug mode
    if (system_status & STATE_USBMODE){
        //Serial print reason and log it
       
    }

};

void delete_message(uint32_t msg){
    system_status &= ~msg;
};
