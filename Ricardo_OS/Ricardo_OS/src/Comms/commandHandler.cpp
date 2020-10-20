#include "commandHandler.h"


#include "stateMachine.h"


CommandHandler::CommandHandler(stateMachine* sm, CommandBuffer* buffer_ptr){
    _sm = sm;
	_buffer_ptr = buffer_ptr;
};

void CommandHandler::setup(){};


void CommandHandler::update() {
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
		case COMMANDS::Callibrate_Baro:
			break;
		case COMMANDS::Callibrate_Gyro:
			break;
		case COMMANDS::Callibrate_Mag:
    		break;
		case COMMANDS::Clear_Flash:
            break;
		case COMMANDS::Clear_SD:
            break;
		case COMMANDS::Detailed_Accel:
            break;
    	case COMMANDS::Detailed_All:
            break;
		case COMMANDS::Detailed_Baro:
            break;
		case COMMANDS::Detailed_Battery:
            break;
		case COMMANDS::Detailed_GPS:
            break;
		case COMMANDS::Detailed_Gyro:
            break;
		case COMMANDS::Detailed_Mag:
            break;
		case COMMANDS::Detailed_Throttle:
            break;
		case COMMANDS::Enter_Countdown:
            break;
		case COMMANDS::Enter_Flight:
            break;
		case COMMANDS::Enter_Groundstation:
            break;
		case COMMANDS::Enter_Recovery:
            break;
		case COMMANDS::Enter_USBMode:
            break;
		case COMMANDS::Estimator_Output:
            break;
		case COMMANDS::Fire_pyro_1:
            break;
		case COMMANDS::Fire_pyro_2:
            break;
		case COMMANDS::Launch:
            break;
		case COMMANDS::Play_Song:
            break;
		case COMMANDS::Print_Flash:
            break;
		case COMMANDS::Print_Sd:
            break;
		case COMMANDS::Raw_Output:
            break;
		case COMMANDS::Reset:
            break;
		case COMMANDS::Set_Throttle:
            break;
		case COMMANDS::Start_Logging:
            break;
		case COMMANDS::Stop_Logging:
            break;
		case COMMANDS::Zero_Sensors:
            break;
	}
};

