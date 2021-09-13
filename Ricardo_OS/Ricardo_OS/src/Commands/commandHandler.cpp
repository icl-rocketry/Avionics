#include "commandHandler.h"

#include <vector>
#include <functional>
#include <memory>


#include "stateMachine.h"
#include "States/usbmode.h"
#include "States/groundstation.h"
#include "States/launch.h"
#include "States/flight.h"
#include "States/recovery.h"
#include "States/preflight.h"


#include "commands.h"

#include "flags.h"


#include "Sound/Melodies/melodyLibrary.h"

#include "Storage/logController.h"

#include "rnp_packet.h"
#include "rnp_interface.h"
#include "Network/interfaces/radio.h"
#include "commandpacket.h"

#include "Network/packets/TelemetryPacket.h"



CommandHandler::CommandHandler(stateMachine* sm):
_sm(sm)
{};

void CommandHandler::handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr) {

	CommandPacket commandpacket(*packetptr);

	switch (static_cast<COMMANDS>(commandpacket.command)) {
		case COMMANDS::Launch:
			LaunchCommand(commandpacket);
			break;
		case COMMANDS::Reset:
			ResetCommand(commandpacket);
			break;
		case COMMANDS::Abort:
			AbortCommand(commandpacket);
			break;
		case COMMANDS::Start_Logging:
			StartLoggingCommand(commandpacket);
			break;
		case COMMANDS::Stop_Logging:
			StopLoggingCommand(commandpacket);
			break;
		case COMMANDS::Telemetry:
			TelemetryCommand(commandpacket);
			break;
		case COMMANDS::Clear_Flash:
			ClearFlashCommand(commandpacket);
			break;
		case COMMANDS::Clear_SD:
			ClearSDCommand(commandpacket);
			break;
		case COMMANDS::Print_Flash_filesystem:
			break;
		case COMMANDS::Print_Sd_filesystem:
			break;
		case COMMANDS::Play_Song:
			PlaySongCommand(commandpacket);
			break;
		case COMMANDS::Skip_Song:
			SkipSongCommand(commandpacket);
			break;
		case COMMANDS::Clear_Song_Queue:
			ClearSongQueueCommand(commandpacket);
			break;
		case COMMANDS::Calibrate_AccelGyro_Bias:
			CalibrateAccelGyroBiasCommand(commandpacket);
			break;
		case COMMANDS::Calibrate_Mag_Bias:
			CalibrateMagBiasCommand(commandpacket);
			break;
		case COMMANDS::Set_Beta:
			SetBetaCommand(commandpacket);
			break;
		case COMMANDS::Reset_Orientation:
			ResetOrientationCommand(commandpacket);
			break;
		case COMMANDS::Reset_Localization:
			ResetLocalizationCommand(commandpacket);
			break;
		case COMMANDS::Enter_USBMode:
			EnterUSBModeCommand(commandpacket);
			break;
		case COMMANDS::Enter_Groundstation:
			EnterGroundstationCommand(commandpacket);
			break;
		case COMMANDS::Enter_Countdown:
			EnterCountdownCommand(commandpacket);
			break;
		case COMMANDS::Enter_Flight:
			EnterFlightCommand(commandpacket);
			break;
		case COMMANDS::Enter_Recovery:
			EnterRecoveryCommand(commandpacket);
			break;
		case COMMANDS::Exit_USBMode:
			ExitUSBModeCommand(commandpacket);
			break;
		case COMMANDS::Exit_to_USBMode:
			ExitUSBModeCommand(commandpacket);
			break;
		case COMMANDS::Set_Throttle:
			SetThrottleCommand(commandpacket);
			break;
		case COMMANDS::Engine_Info:
			EngineInfoCommand(commandpacket);
			break;
		case COMMANDS::Pyro_info:
			PyroInfoCommand(commandpacket);
			break;
		case COMMANDS::Fire_pyro:
			FireInfoCommand(commandpacket);
			break;
		default:
			//invalid command issued DELETE IT 
			break;
			
	};
	
}

std::function<void(std::unique_ptr<RnpPacketSerialized>)> CommandHandler::getCallback() 
{
	return std::bind(&CommandHandler::handleCommand, this, std::placeholders::_1);
};



void CommandHandler::LaunchCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->changeState(new Launch(_sm));
}

void CommandHandler::ResetCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_RECOVERY,SYSTEM_FLAG::STATE_GROUNDSTATION)){
		return;
	}
	_sm->changeState(new Preflight(_sm));
}

void CommandHandler::AbortCommand(const  CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_LAUNCH,SYSTEM_FLAG::STATE_FLIGHT)){
		return;
	}
	if(_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_LAUNCH)){
		//check if we are in no abort time region
		//close all valves
		_sm->changeState(new Preflight(_sm));
	}else if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_FLIGHT)){
		//this behaviour needs to be confirmed with recovery 
		//might be worth waiting for acceleration to be 0 after rocket engine cut
		_sm->changeState(new Recovery(_sm));
	}
}

void CommandHandler::SetHomeCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	
}

void CommandHandler::StartLoggingCommand(const CommandPacket &commandpacket) 
{
	_sm->logcontroller.startLogging((LOG_TYPE)commandpacket.arg);
}

void CommandHandler::StopLoggingCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->logcontroller.stopLogging((LOG_TYPE)commandpacket.arg);
}

void CommandHandler::TelemetryCommand(const CommandPacket &commandpacket) 
{
	//std::vector<uint8_t> packet;
	TelemetryPacket telemetry;

	telemetry.header.source = _sm->networkmanager.getAddress();
	telemetry.header.destination = commandpacket.header.source;
	telemetry.header.uid = commandpacket.header.uid; 

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

	telemetry.baro_temp = _sm->sensors.sensors_raw.baro_temp;
	telemetry.baro_press = _sm->sensors.sensors_raw.baro_press;
	telemetry.baro_alt = _sm->sensors.sensors_raw.baro_alt;

	telemetry.batt_voltage = _sm->sensors.sensors_raw.batt_volt;
	telemetry.batt_percent= _sm->sensors.sensors_raw.batt_percent;

	telemetry.launch_lat = _sm->estimator.state.gps_launch_lat;
	telemetry.launch_lng = _sm->estimator.state.gps_launch_long;
	telemetry.launch_alt = _sm->estimator.state.gps_launch_alt;

	telemetry.system_status = _sm->systemstatus.getStatus();
	telemetry.system_time = millis();

	//std::vector<double> radioInfo = _sm->networkmanager.getInterfaceInfo(INTERFACE::LORA);

	const RadioInterfaceInfo* radioinfo = static_cast<const RadioInterfaceInfo*>(_sm->radio.getInfo());
	telemetry.rssi = radioinfo->rssi;
	telemetry.snr = radioinfo->snr;



	_sm->networkmanager.sendPacket(telemetry);

}

void CommandHandler::ClearFlashCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::STATE_GROUNDSTATION,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->storagecontroller.erase(STORAGE_DEVICE::FLASH);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete

}

void CommandHandler::ClearSDCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::STATE_GROUNDSTATION,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->storagecontroller.erase(STORAGE_DEVICE::MICROSD);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::PlaySongCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->tunezhandler.play_by_idx(commandpacket.arg);
}

void CommandHandler::SkipSongCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->tunezhandler.skip();
}

void CommandHandler::ClearSongQueueCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->tunezhandler.clear();
}

void CommandHandler::ResetOrientationCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
}

void CommandHandler::ResetLocalizationCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->estimator.setup();
}

void CommandHandler::SetBetaCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	float beta = ((float)commandpacket.arg) / 100.0;
	_sm->estimator.changeBeta(beta);
}

void CommandHandler::CalibrateAccelGyroBiasCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->sensors.calibrate(SENSOR::ACCELGYRO);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::CalibrateMagBiasCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->sensors.calibrate(SENSOR::MAG);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::EnterUSBModeCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
	_sm->systemstatus.new_message(SYSTEM_FLAG::DEBUG);
}

void CommandHandler::EnterGroundstationCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Groundstation(_sm));
}

void CommandHandler::EnterCountdownCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Launch(_sm));
}

void CommandHandler::EnterFlightCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Flight(_sm));
}

void CommandHandler::EnterRecoveryCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Recovery(_sm));
}

void CommandHandler::ExitUSBModeCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
	_sm->systemstatus.delete_message(SYSTEM_FLAG::DEBUG);
	_sm->changeState(new Preflight(_sm));
}

void CommandHandler::ExitToUSBModeCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
}

void CommandHandler::EngineInfoCommand(const CommandPacket &commandpacket) 
{
	
}

void CommandHandler::SetThrottleCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_USBMODE)){
		return;
	}
}

void CommandHandler::PyroInfoCommand(const CommandPacket &commandpacket) 
{
	
}

void CommandHandler::FireInfoCommand(const CommandPacket &commandpacket) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_USBMODE)){
		return;
	}
};


