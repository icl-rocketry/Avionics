#include <Arduino.h>


// message type: INFO, ERROR
// importance: 1-logged on main_log; 2-broadcast over serial, logged on error_log; 3-audible warn, logged in critical_log
void message(uint8_t type, bool log, bool alarm, String message);