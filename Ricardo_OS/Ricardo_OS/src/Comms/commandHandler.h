//process commands stored in command buffer and execute

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include <Arduino.h>
#include "commandBuffer.h"


class stateMachine;//forward declaration to prevent circular dependancy

class CommandHandler {
    public:
        CommandHandler(stateMachine* sm, CommandBuffer* buffer_ptr);

        void setup();
        void update();


    private:
        stateMachine* _sm; //pointer to state machine
		CommandBuffer* _buffer_ptr;
		
		uint8_t* handleCommand(Command command);
};	

#endif