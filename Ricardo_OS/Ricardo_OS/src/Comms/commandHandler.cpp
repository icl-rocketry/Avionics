#include "commandHandler.h"


#include "stateMachine.h"
#include "States/usbmode.h"
#include "States/groundstation.h"
#include "States/countdown.h"
#include "States/flight.h"
#include "States/recovery.h"


#include "commands.h"

#include "flags.h"

#include "packets.h"
#include "interfaces/interfaces.h"



CommandHandler::CommandHandler(stateMachine* sm):
	_sm(sm)
	{
 
};


void CommandHandler::setup(){};


void CommandHandler::update() {
	// Handle the first command in the buffer
	// Possibly implement priority queues in the future?
	if (commandbuffer.size()>0){
		Command first_command = commandbuffer.front();
		handleCommand(first_command);
		commandbuffer.erase(commandbuffer.begin()); // 0th command handled, remove it from the buffer
	};
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
				_sm->changeState(new Countdown(_sm));
				break;
			case COMMANDS::Reset:
				_sm->changeState(new Preflight(_sm));
				break;
			case COMMANDS::Abort:
				if(_sm->systemstatus.flag_triggered(system_flag::STATE_COUNTDOWN)){
					//check if we are in no abort time region
					//close all valves
					_sm->changeState(new Preflight(_sm));
				}else if (_sm->systemstatus.flag_triggered(system_flag::STATE_FLIGHT)){
					//this behaviour needs to be confirmed with recovery 
					//might be worth waiting for acceleration to be 0 after rocket engine cut
					_sm->changeState(new Recovery(_sm));
				}
				break;
			case COMMANDS::Zero_Sensors:
				break;
			case COMMANDS::Start_Logging:
				break;
			case COMMANDS::Stop_Logging:
				break;
			case COMMANDS::Play_Song:
				break;
			case COMMANDS::Telemetry:
				break;
			case COMMANDS::Clear_Flash:
				break;
			case COMMANDS::Clear_SD:
				break;
			case COMMANDS::Print_Flash_filesystem:
				break;
			case COMMANDS::Print_Sd_filesystem:
				break;
			case COMMANDS::Raw_Sensors:
				break;
			case COMMANDS::Detailed_All_Sensors:
				{
					std::vector<uint8_t> packet;

					DetailedAllPacket detailedall = DetailedAllPacket();

					detailedall.header.source = _sm->networkmanager.get_node_type();
					detailedall.header.destination = static_cast<uint8_t>(command.source_node);



					detailedall.ax = _sm->sensors.sensors_raw.ax;
					detailedall.ay = _sm->sensors.sensors_raw.ay;
					detailedall.az = _sm->sensors.sensors_raw.az;
					detailedall.gx = _sm->sensors.sensors_raw.gx;
					detailedall.gy = _sm->sensors.sensors_raw.gy;
					detailedall.gz = _sm->sensors.sensors_raw.gz;
					detailedall.mx = _sm->sensors.sensors_raw.mx;
					detailedall.my = _sm->sensors.sensors_raw.my;
					detailedall.mz = _sm->sensors.sensors_raw.mz;
					

					detailedall.serialize(packet);
					
					_sm->networkmanager.send_to_node(command.source_node,packet.data(),detailedall.header.packet_len+detailedall.header.header_len);
					
				}
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
			//Transitions from pre-flight to USBMode,toggles debug flag in state machine							
				_sm->changeState(new USBmode(_sm));
				_sm->systemstatus.new_message(system_flag::DEBUG);

				break;
			case COMMANDS::Enter_Groundstation:
				_sm->changeState(new Groundstation(_sm));
				break;
			case COMMANDS::Enter_Countdown:
				_sm->changeState(new Countdown(_sm));
				break;
			case COMMANDS::Enter_Flight:
				_sm->changeState(new Flight(_sm));
				break;
			case COMMANDS::Enter_Recovery:
				_sm->changeState(new Recovery(_sm));
				break;
			case COMMANDS::Exit_USBMode:
			//Transitions from any state in debug mode first to USBmode, then to preflight
				{
				_sm->changeState(new USBmode(_sm));
				_sm->systemstatus.delete_message(system_flag::DEBUG);
				_sm->changeState(new Preflight(_sm));
				}
				break;
			case COMMANDS::Exit_to_USBMode:
			//Transtions from any state in debug to USBMode
				{
				_sm->changeState(new USBmode(_sm));
				}
				break;
			case COMMANDS::Set_Throttle:
				break;
			case COMMANDS::Pyro_info:
				{
				uint8_t pyro_number = command.arg;
				}
				break;
			case COMMANDS::Fire_pyro:
				{
				uint8_t pyro_number = command.arg;
				}
				break;
			default:
				//invalid command issued DELETE IT 
				break;
				
		};
	};
};


bool CommandHandler::commandAvaliable(Command command) {
		// Checks if a comand can be executed in a given state
	switch(command.type){
		case COMMANDS::Start_Logging:
		case COMMANDS::Telemetry:
		case COMMANDS::Pyro_info:
    		return true; // all states
		case COMMANDS::Abort:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_COUNTDOWN) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_FLIGHT);
		case COMMANDS::Enter_Countdown:
		case COMMANDS::Enter_Flight:
		case COMMANDS::Enter_Recovery:
		case COMMANDS::Exit_USBMode:
		case COMMANDS::Exit_to_USBMode:
		case COMMANDS::Set_Throttle:
            return _sm->systemstatus.flag_triggered(system_flag::DEBUG);
		case COMMANDS::Clear_Flash:
    	case COMMANDS::Clear_SD:
		case COMMANDS::Print_Flash_filesystem:
		case COMMANDS::Print_Sd_filesystem:
            return _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION)
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT)
			|| _sm->systemstatus.flag_triggered(system_flag::DEBUG);
		case COMMANDS::Launch:
		case COMMANDS::Enter_USBMode:
		case COMMANDS::Play_Song:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT);
		case COMMANDS::Raw_Sensors:
		case COMMANDS::Detailed_All_Sensors:
		case COMMANDS::Callibrate_Accel:
		case COMMANDS::Callibrate_Mag:
		case COMMANDS::Callibrate_Gyro:
		case COMMANDS::Callibrate_Baro:
		case COMMANDS::Enter_Groundstation:
		case COMMANDS::Stop_Logging:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT) 
		||  _sm->systemstatus.flag_triggered(system_flag::DEBUG);
		case COMMANDS::Zero_Sensors:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION) 
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_PREFLIGHT)
			|| _sm->systemstatus.flag_triggered(system_flag::DEBUG);
		case COMMANDS::Reset:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_RECOVERY)
			|| _sm->systemstatus.flag_triggered(system_flag::STATE_GROUNDSTATION);
		case COMMANDS::Fire_pyro:
			return _sm->systemstatus.flag_triggered(system_flag::STATE_USBMODE);
		default:
			return false;
	}

};

