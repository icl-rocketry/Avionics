#include "MessageHandler.h"
#include "config.h"



// message type: 0- INFO, 1 - ERROR , 2 - CRITCAL


void message(uint8_t type, bool log, bool alarm, String message){
    int64_t sys_time = esp_timer_get_time();
    if (type == 1){
        message = "[E]" + message;       
    }
    else if (type == 2){
        message = "[C]" + message;
    }
    else{
        message = "[I]" + message;
    }
    if (log){
        message = String(sys_time) + ':' + message;    
        //log to messages log
        
    }
    if (alarm){
        //make sound using type of message to define sound
    }
    #ifdef VERBOSE
    message = String(sys_time) + ':' + message;    
    Serial.println(message);
    #endif

}