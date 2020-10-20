#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <Arduino.h>
#include "commands.h"




struct Command {
	uint8_t interface;
	COMMANDS type;
};

class CommandBuffer {

	public:
		std::vector<Command> buffer;


		void addCommand(Command);
	
	
	

};

#endif