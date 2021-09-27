/*
handles all messages as a bitfield so multiple messages can be contained in a single variable
*/

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "flags.h"
#include "logController.h"

#include <string>


class SystemStatus{
    public:
        
        SystemStatus(LogController* logcontroller);
        void new_message(SYSTEM_FLAG flag,std::string info);
        void new_message(SYSTEM_FLAG flag);

        uint32_t getStatus();

        void delete_message(SYSTEM_FLAG flag);
        void delete_message(SYSTEM_FLAG flag,std::string info);

        template<typename... Args> 
        /**
         * @brief Checks if multiple flags have been triggered e.g flag1 or flag2 or ... 
         * 
         * @param args 
         * @return true 
         * @return false 
         */
        bool flag_triggered(Args... args){
            uint32_t flags = (... | static_cast<uint32_t>(args));
            return (_status & flags);
        };

    private:
        LogController* _logcontroller;//pointer to logcontroller
        uint32_t _status;
};

#endif