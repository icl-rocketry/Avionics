//process commands stored in command buffer and execute

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include <Arduino.h>
#include <vector>

//#include "commandBuffer.h"
#include "commands.h"

#include "interfaces/interfaces.h"
#include "nodes.h"



class stateMachine;//forward declaration to prevent circular dependancy

struct Command {
	NODES source_node; //source node 
	COMMANDS type; 
    uint8_t arg;
    uint32_t uid; // unique id 
};

class CommandHandler {

    friend class NetworkManager;

    public:
        CommandHandler(stateMachine* sm);

        void setup();
        void update();


    private:
        stateMachine* _sm; //pointer to state machine

        std::vector<Command> commandbuffer;
		
        void addCommand(Command command);

		void handleCommand(Command command);
        bool commandAvaliable(Command command);
};	

#endif