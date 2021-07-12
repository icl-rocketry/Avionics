#include "commandHandler.h"

#include <vector>

#include "stateMachine.h"
#include "States/usbmode.h"
#include "States/groundstation.h"
#include "States/launch.h"
#include "States/flight.h"
#include "States/recovery.h"


#include "commands.h"

#include "flags.h"

#include "packets.h"
#include "interfaces/interfaces.h"
#include "nodes.h"
#include "interfaces/radio.h"

#include "Sound/Melodies/melodyLibrary.h"



CommandHandler::CommandHandler(stateMachine* sm):
_sm(sm)
{};



void CommandHandler::handleCommand(const CommandPacket &commandpacket) {

	COMMANDS command_id = static_cast<COMMANDS>(commandpacket.command);
	uint8_t arg = commandpacket.arg;
	NODES source = static_cast<NODES>(commandpacket.header.source);
	uint32_t uid = commandpacket.header.uid;

	if (commandAvaliable(command_id)) {
		switch (command_id) {
			case COMMANDS::Launch:
				_sm->changeState(new Launch(_sm));
				break;
			case COMMANDS::Reset:
				_sm->changeState(new Preflight(_sm));
				break;
			case COMMANDS::Abort:
				if(_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_LAUNCH)){
					//check if we are in no abort time region
					//close all valves
					_sm->changeState(new Preflight(_sm));
				}else if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_FLIGHT)){
					//this behaviour needs to be confirmed with recovery 
					//might be worth waiting for acceleration to be 0 after rocket engine cut
					_sm->changeState(new Recovery(_sm));
				}
				break;
			case COMMANDS::Zero_Sensors:
				break;
			case COMMANDS::Start_Logging:
				_sm->logcontroller.startLogging(LOG_TYPE::TELEMETRY);
				break;
			case COMMANDS::Stop_Logging:
				_sm->logcontroller.stopLogging(LOG_TYPE::TELEMETRY);
				break;
			case COMMANDS::Telemetry:
				{
					std::vector<uint8_t> packet;
					TelemetryPacket telemetry;

					telemetry.header.source = _sm->networkmanager.getNodeType();
					telemetry.header.destination = (uint8_t)source;
					telemetry.header.uid = uid; 

					telemetry.pn = _sm->estimator.state.position(0);
					telemetry.pe = _sm->estimator.state.position(1);
					telemetry.pd = _sm->estimator.state.position(2);

					telemetry.vn = _sm->estimator.state.velocity(0);
					telemetry.ve = _sm->estimator.state.velocity(1);
					telemetry.vd = _sm->estimator.state.velocity(2);

					telemetry.an = _sm->estimator.state.acceleration(0);
					telemetry.ae = _sm->estimator.state.acceleration(1);
					telemetry.ad = _sm->estimator.state.acceleration(2);

					telemetry.roll = _sm->estimator.state.eulerAngles(0);
					telemetry.pitch = _sm->estimator.state.eulerAngles(1);
					telemetry.yaw =_sm->estimator.state.eulerAngles(2);

					telemetry.q0 = _sm->estimator.state.orientation.w();
					telemetry.q1 = _sm->estimator.state.orientation.x();
					telemetry.q2 =_sm->estimator.state.orientation.y();
					telemetry.q3 =_sm->estimator.state.orientation.z();

					telemetry.lat = _sm->sensors.sensors_raw.gps_lat / 10000000.0;
					telemetry.lng = _sm->sensors.sensors_raw.gps_long / 10000000.0;
					telemetry.alt = _sm->sensors.sensors_raw.gps_alt;
					telemetry.sat = _sm->sensors.sensors_raw.gps_sat;

					telemetry.ax = _sm->sensors.sensors_raw.ax;
					telemetry.ay = _sm->sensors.sensors_raw.ay;
					telemetry.az = _sm->sensors.sensors_raw.az;

					telemetry.gx = _sm->sensors.sensors_raw.gx;
					telemetry.gy = _sm->sensors.sensors_raw.gy;
					telemetry.gz = _sm->sensors.sensors_raw.gz;

					telemetry.mx = _sm->sensors.sensors_raw.mx;
					telemetry.my = _sm->sensors.sensors_raw.my;
					telemetry.mz = _sm->sensors.sensors_raw.mz;

					telemetry.temp = _sm->sensors.sensors_raw.baro_temp;
					telemetry.press = _sm->sensors.sensors_raw.baro_press;

					telemetry.batt_voltage = _sm->sensors.sensors_raw.batt_volt;
					telemetry.batt_percent= _sm->sensors.sensors_raw.batt_percent;

					telemetry.launch_lat = _sm->estimator.state.gps_launch_lat;
					telemetry.launch_lng = _sm->estimator.state.gps_launch_long;
					telemetry.launch_alt = _sm->estimator.state.gps_launch_alt;

					telemetry.system_status = _sm->systemstatus.get_string();
					telemetry.system_time = millis();

					std::vector<double> radioInfo = _sm->networkmanager.getInterfaceInfo(INTERFACE::LORA);

					telemetry.rssi = radioInfo.at((uint8_t)RADIO_INFO::RSSI);
					telemetry.snr = radioInfo.at((uint8_t)RADIO_INFO::SNR);

					telemetry.serialize(packet);

					_sm->networkmanager.send_to_node(source,packet);

				}
				break;
			case COMMANDS::Clear_Flash:
				_sm->storagecontroller.erase(STORAGE_DEVICE::FLASH);
				_sm->tunezhandler.play(MELODY::CONFIRMATION); //play sound when complete
				break;
			case COMMANDS::Clear_SD:
				_sm->storagecontroller.erase(STORAGE_DEVICE::MICROSD);
				_sm->tunezhandler.play(MELODY::CONFIRMATION); //play sound when complete
				break;
			case COMMANDS::Print_Flash_filesystem:
				break;
			case COMMANDS::Print_Sd_filesystem:
				break;
			case COMMANDS::Play_Song:
			{	
				_sm->tunezhandler.play(static_cast<MELODY>(arg)); 
				break;
			}
			case COMMANDS::Skip_Song:
				_sm->tunezhandler.skip(); // play startup sound
				break;
			case COMMANDS::Clear_Song_Queue:
				_sm->tunezhandler.clear(); // play startup sound
				break;
			case COMMANDS::Raw_Sensors:
				break;
			case COMMANDS::Detailed_All_Sensors:
				{
					std::vector<uint8_t> packet;

					DetailedAllPacket detailedall;

					detailedall.header.source = _sm->networkmanager.getNodeType();
					detailedall.header.destination = (uint8_t)source;
					detailedall.header.uid = uid; 

					detailedall.ax = _sm->sensors.sensors_raw.ax;
					detailedall.ay = _sm->sensors.sensors_raw.ay;
					detailedall.az = _sm->sensors.sensors_raw.az;

					detailedall.gx = _sm->sensors.sensors_raw.gx;
					detailedall.gy = _sm->sensors.sensors_raw.gy;
					detailedall.gz = _sm->sensors.sensors_raw.gz;

					detailedall.mx = _sm->sensors.sensors_raw.mx;
					detailedall.my = _sm->sensors.sensors_raw.my;
					detailedall.mz = _sm->sensors.sensors_raw.mz;

					detailedall.gps_lat = _sm->sensors.sensors_raw.gps_lat / 10000000.0;
					detailedall.gps_long = _sm->sensors.sensors_raw.gps_long / 10000000.0;
					
					detailedall.gps_speed = _sm->sensors.sensors_raw.gps_sat;

					detailedall.gps_alt = _sm->sensors.sensors_raw.gps_alt;

					detailedall.baro_alt = _sm->sensors.sensors_raw.baro_alt;
					detailedall.baro_temp = _sm->sensors.sensors_raw.baro_temp;
					detailedall.baro_press = _sm->sensors.sensors_raw.baro_press;

					detailedall.batt_volt = _sm->sensors.sensors_raw.batt_volt;
					detailedall.batt_percent= _sm->sensors.sensors_raw.batt_percent;

					detailedall.serialize(packet);

					_sm->networkmanager.send_to_node(source,packet);
					
				}
				break;
			case COMMANDS::Calibrate_AccelGyro:
				_sm->sensors.calibrate(SENSOR::ACCELGYRO);
				_sm->tunezhandler.play(MELODY::CONFIRMATION); //play sound when complete
				break;
			case COMMANDS::Calibrate_Mag:
				_sm->sensors.calibrate(SENSOR::MAG);
				_sm->tunezhandler.play(MELODY::CONFIRMATION); //play sound when complete
				break;
			case COMMANDS::Calibrate_Baro:
				{
				//set beta for testing
				float beta = ((float)arg) / 100.0;
				_sm->estimator.changeBeta(beta);
				break;
				}
			case COMMANDS::Enter_USBMode:
			//Transitions from pre-flight to USBMode,toggles debug flag in state machine							
				_sm->changeState(new USBmode(_sm));
				_sm->systemstatus.new_message(SYSTEM_FLAG::DEBUG);
				break;
			case COMMANDS::Enter_Groundstation:
				_sm->changeState(new Groundstation(_sm));
				break;
			case COMMANDS::Enter_Countdown:
				_sm->changeState(new Launch(_sm));
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
				_sm->systemstatus.delete_message(SYSTEM_FLAG::DEBUG);
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
				uint8_t pyro_number = arg;
				}
				break;
			case COMMANDS::Fire_pyro:
				{
				uint8_t pyro_number = arg;
				}
				break;
			default:
				//invalid command issued DELETE IT 
				break;
				
		};
	};
};


bool CommandHandler::commandAvaliable(COMMANDS command) {
		// Checks if a comand can be executed in a given state
	switch(command){
		case COMMANDS::Start_Logging:
		case COMMANDS::Telemetry:
		case COMMANDS::Pyro_info:
    		return true; // all states
		case COMMANDS::Abort:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_LAUNCH) 
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_FLIGHT);
		case COMMANDS::Enter_Countdown:
		case COMMANDS::Enter_Flight:
		case COMMANDS::Enter_Recovery:
		case COMMANDS::Exit_USBMode:
		case COMMANDS::Exit_to_USBMode:
		case COMMANDS::Set_Throttle:
            return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG);
		case COMMANDS::Clear_Flash:
    	case COMMANDS::Clear_SD:
		case COMMANDS::Print_Flash_filesystem:
		case COMMANDS::Print_Sd_filesystem:
            return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_GROUNDSTATION)
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG);
		case COMMANDS::Launch:
		case COMMANDS::Enter_USBMode:
		case COMMANDS::Play_Song:
		case COMMANDS::Skip_Song:
		case COMMANDS::Clear_Song_Queue:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT);
		case COMMANDS::Raw_Sensors:
		case COMMANDS::Detailed_All_Sensors:
		case COMMANDS::Calibrate_AccelGyro:
		case COMMANDS::Calibrate_Mag:
		case COMMANDS::Calibrate_Baro:
		case COMMANDS::Enter_Groundstation:
		case COMMANDS::Stop_Logging:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT) 
		||  _sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)
		|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_GROUNDSTATION); //for testing
		case COMMANDS::Zero_Sensors:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_GROUNDSTATION) 
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG);
		case COMMANDS::Reset:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_RECOVERY)
			|| _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_GROUNDSTATION);
		case COMMANDS::Fire_pyro:
			return _sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_USBMODE);
		default:
			return false;
	}

};

