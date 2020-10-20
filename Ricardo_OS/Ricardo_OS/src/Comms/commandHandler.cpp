#include "commandHandler.h"


CommandHandler::CommandHandler(stateMachine* sm, CommandBuffer* buffer_ptr){
    _sm = sm;
	_buffer_ptr = buffer_ptr;
};

CommandHandler::update() {
	// Handle the first command in the buffer
	// Possibly implement priority queues in the future?
	Command first_command = _buffer_ptr->at(0);
	uint8_t* data = handleCommand(first_command);
	if (data) {
		// If there's return data, send it back through the requesting interface
		_sm->downlink.send_data(first_command.interface_id, data, sizeof(*data) / 8); 
	}
	_buffer_ptr->erase(0); // 0th command handled, remove it from the buffer
};

uint8_t* CommandHandler::handleCommand(Command command) {

	switch (command.type) {
		case COMMANDS::Abort:
			break;
		case COMMANDS::Callibrate_Accel:
			break;
		case Callibrate_Baro:
			break;
		case Callibrate_Gyro:
			break;
		case Callibrate_Mag:
    		break;
		case Clear_Flash:
            break;
		case Clear_SD:
            break;
		case Detailed_Accel:
            break;
    	case Detailed_All:
            break;
		case Detailed_Baro:
            break;
		case Detailed_Battery:
            break;
		case Detailed_GPS:
            break;
		case Detailed_Gyro:
            break;
		case Detailed_Mag:
            break;
		case Detailed_Throttle:
            break;
		case Enter_Countdown:
            break;
		case Enter_Flight:
            break;
		case Enter_Groundstation:
            break;
		case Enter_Recovery:
            break;
		case Enter_USBMode:
            break;
		case Estimator_Output:
            break;
		case Fire_pyro_1:
            break;
		case Fire_pyro_2:
            break;
		case Launch:
            break;
		case Play_Song:
            break;
		case Print_Flash:
            break;
		case Print_Sd:
            break;
		case Raw_Output:
            break;
		case Reset:
            break;
		case Set_Throttle:
            break;
		case Start_Logging:
            break;
		case Stop_Logging:
            break;
		case Zero_Sensors:
            break;
	}
};

