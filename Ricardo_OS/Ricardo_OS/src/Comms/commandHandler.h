//process commands stored in command buffer and execute

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include <Arduino.h>
#include <vector>
#include "commands.h"
#include "packets.h"


#include "interfaces/interfaces.h"
#include "nodes.h"





class stateMachine;//forward declaration to prevent circular dependancy

class CommandHandler {
    public:
        CommandHandler(stateMachine* sm);
        void handleCommand(const CommandPacket &commandpacket);


    private:
        stateMachine* _sm; //pointer to state machine
        bool commandAvaliable(COMMANDS command);
};	

#endif


