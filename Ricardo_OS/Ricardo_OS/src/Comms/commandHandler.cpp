#include "commandHandler.h"


#include "stateMachine.h"

#include "commands.h"

#include "flags.h"

#include "packets.h"
#include "interfaces/interfaces.h"

CommandHandler::CommandHandler(stateMachine* sm){
    _sm = sm;
};


void CommandHandler::setup(){};


void CommandHandler::update() {
	// Handle the first command in the buffer
	// Possibly implement priority queues in the future?
	Command first_command = commandbuffer.front();
	handleCommand(first_command);

	commandbuffer.erase(commandbuffer.begin()); // 0th command handled, remove it from the buffer
};

void CommandHandler::addCommand(Command command){
	commandbuffer.push_back(command);
};

void CommandHandler::handleCommand(Command command) {

	if (!commandAvaliable(command)) {
		//do nothing as command is not avaliable
	} else{
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
				{
					std::vector<uint8_t> packet;

					DetailedAllPacket detailedall = DetailedAllPacket();

					detailedall.header.source = _sm->networkmanager.get_node_type();
					detailedall.header.destination = static_cast<uint8_t>(command.source_node);



					detailedall.ax = _sm->estimator.state.ax;
					detailedall.ay = _sm->estimator.state.ay;
					detailedall.az = _sm->estimator.state.az;

					detailedall.serialize(packet);
					
					_sm->networkmanager.send_to_node(command.source_node,packet.data(),detailedall.header.packet_len);
					
				}
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
				
		};
	};
};


bool CommandHandler::commandAvaliable(Command command) {
		// Checks if a comand can be executed in a given state
	switch(command.type){
		case COMMANDS::Launch:
		case COMMANDS::Enter_USBMode:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT);
		case COMMANDS::Reset:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_RECOVERY);
		case COMMANDS::Abort:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_COUNTDOWN) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_FLIGHT);
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
			return _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT)
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_USBMODE);
		case COMMANDS::Stop_Logging:
		return _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT) 
		||  _sm->systemstatus.flag_triggered(system_flag::STATE_USBMODE);
		case COMMANDS::Start_Logging:
    		return true; // all states
		case COMMANDS::Play_Song:
            return _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_USBMODE)
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_RECOVERY);
		case COMMANDS::Clear_Flash:
    	case COMMANDS::Clear_SD:
            return _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION)
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT);
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
            return _sm->systemstatus.flag_triggered(system_flag::STATE_USBMODE);
		default:
			return false;
	}

};

