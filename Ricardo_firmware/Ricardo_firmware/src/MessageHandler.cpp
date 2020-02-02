#include "MessageHandler.h"


// message type: INFO, ERROR
// importance: 1-logged on main_log; 2-broadcast over serial, logged on error_log; 3-audible warn, logged in critical_log
// time since startup uint64_t esp_timer_get_time()
void message(uint8_t type, uint8_t importance, String message, String location){
    int64_t sys_time = esp_timer_get_time();


}