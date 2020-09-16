#include "messages.h"
#include "config.h"

//system status variable intialized at 0
uint32_t system_status = 0;

void new_message(uint32_t msg,String reason){

    system_status |= msg;

    //detect debug mode
    if (flag_triggered(STATE_USBMODE)){
        //Serial print reason and log it
       Serial.println(reason);
    };

};

void delete_message(uint32_t msg){
    system_status &= ~msg;
};

bool flag_triggered(uint32_t msg){
    return (system_status & msg);
};