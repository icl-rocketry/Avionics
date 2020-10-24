#include "commandHandler.h"


#include "stateMachine.h"
#include "commandBuffer.h"
#include "commands.h"

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
		//maybe change return type to NULL?

		// If there's return data, send it back through the requesting interface
		_sm->downlink.send_data(first_command.interface, data, sizeof(*data) / 8); 
	}
	_buffer_ptr->erase(0); // 0th command handled, remove it from the buffer
};

uint8_t* CommandHandler::handleCommand(Command command) {
	uint8_t curr_stateID = _sm->get_currStateID();
	
	switch (command.type) {
		case COMMANDS::Launch:
			break;
		case COMMANDS::Reset:
			break;
		case COMMANDS::Abort:
			break;
		case COMMANDS::Zero_Sensors:
			break;
		case COMMANDS::Start_Logging:
    		break;
		case COMMANDS::Stop_Logging:
            break;
		case COMMANDS::Play_Song:
            break;
		case COMMANDS::Clear_Flash:
            break;
    	case COMMANDS::Clear_SD:
            break;
		case COMMANDS::Print_Flash:
            break;
		case COMMANDS::Print_Sd:
            break;
		case COMMANDS::Estimator_Output:
            break;
		case COMMANDS::Raw_Sensor_Output:
            break;
		case COMMANDS::Detailed_All_Sensors:
            break;
		case COMMANDS::Detailed_Accel:
            break;
		case COMMANDS::Detailed_Gyro:
            break;
		case COMMANDS::Detailed_Mag:
            break;
		case COMMANDS::Detailed_Baro:
            break;
		case COMMANDS::Detailed_GPS:
            break;
		case COMMANDS::Detailed_Battery:
            break;
		case COMMANDS::Detailed_Throttle:
            break;
		case COMMANDS::Callibrate_Accel:
            break;
		case COMMANDS::Callibrate_Mag:
            break;
		case COMMANDS::Callibrate_Gyro:
            break;
		case COMMANDS::Callibrate_Baro:
            break;
		case COMMANDS::Enter_USBMode:
            break;
		case COMMANDS::Enter_Groundstation:
            break;
		case COMMANDS::Enter_Countdown:
            break;
		case COMMANDS::Enter_Flight:
            break;
		case COMMANDS::Enter_Recovery:
            break;
		case COMMANDS::Set_Throttle:
            break;
		case COMMANDS::Fire_pyro_1:
            break;
		case COMMANDS::Fire_pyro_2:
            break;
		default:
			//invalid command issued
			break;

	}

	bool CommandHandler::commandAvailable(Command command, State state) {
		// Checks if a comand can be executed in a given state
		case COMMANDS::Launch:
		case COMMANDS::Enter_USBMode:
			return state == state::PREFLIGHT;
		case COMMANDS::Reset:
			return state == state::RECOVERY;
		case COMMANDS::Abort:
			return state == state::COUNTDOWN || state == state::FLIGHT;
		case COMMANDS::Zero_Sensors:
		case COMMANDS::Detailed_All_Sensors:
		case COMMANDS::Detailed_Accel:
		case COMMANDS::Detailed_Gyro:
		case COMMANDS::Detailed_Mag:
		case COMMANDS::Detailed_Baro:
		case COMMANDS::Detailed_GPS:
		case COMMANDS::Detailed_Battery:
		case COMMANDS::Detailed_Throttle:
		case COMMANDS::Callibrate_Accel:
		case COMMANDS::Callibrate_Mag:
		case COMMANDS::Callibrate_Gyro:
		case COMMANDS::Callibrate_Baro:
			return state == state::GROUNDSTATION || state == state::PREFLIGHT || return state == state::USBMODE;
		case COMMANDS::Stop_Logging:
		return state == state::PREFLIGHT || return state == state::USBMODE;
		case COMMANDS::Start_Logging:
    		return true;
		case COMMANDS::Play_Song:
            return state == state::GROUNDSTATION || state == state::PREFLIGHT || state == state::USBMODE || state == state::RECOVERY;
		case COMMANDS::Clear_Flash:
    	case COMMANDS::Clear_SD:
            return state == state::GROUNDSTATION || state == state::PREFLIGHT;
		case COMMANDS::Print_Flash:
		case COMMANDS::Print_Sd:
		case COMMANDS::Estimator_Output:
		case COMMANDS::Raw_Sensor_Output:
		case COMMANDS::Enter_Groundstation:
		case COMMANDS::Enter_Countdown:
		case COMMANDS::Enter_Flight:
		case COMMANDS::Enter_Recovery:
		case COMMANDS::Set_Throttle:
		case COMMANDS::Fire_pyro_1:
		case COMMANDS::Fire_pyro_2:
            return state == state::USBMODE;
		default:
			return false;
	}

};

