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


	switch (static_cast<COMMANDS>(CommandPacket::getCommand(*packetptr))) {
		case COMMANDS::Launch:
			LaunchCommand(*packetptr);
			break;
		case COMMANDS::Reset:
			ResetCommand(*packetptr);
			break;
		case COMMANDS::Abort:
			AbortCommand(*packetptr);
			break;
		case COMMANDS::Start_Logging:
			StartLoggingCommand(*packetptr);
			break;
		case COMMANDS::Stop_Logging:
			StopLoggingCommand(*packetptr);
			break;
		case COMMANDS::Telemetry:
			TelemetryCommand(*packetptr);
			break;
		case COMMANDS::Clear_Flash:
			ClearFlashCommand(*packetptr);
			break;
		case COMMANDS::Clear_SD:
			ClearSDCommand(*packetptr);
			break;
		case COMMANDS::Print_Flash_filesystem:
			break;
		case COMMANDS::Print_Sd_filesystem:
			break;
		case COMMANDS::Play_Song:
			PlaySongCommand(*packetptr);
			break;
		case COMMANDS::Skip_Song:
			SkipSongCommand(*packetptr);
			break;
		case COMMANDS::Clear_Song_Queue:
			ClearSongQueueCommand(*packetptr);
			break;
		case COMMANDS::Calibrate_AccelGyro_Bias:
			CalibrateAccelGyroBiasCommand(*packetptr);
			break;
		case COMMANDS::Calibrate_Mag_Bias:
			CalibrateMagBiasCommand(*packetptr);
			break;
		case COMMANDS::Calibrate_Mag_Full:
			CalibrateMagFullCommand(*packetptr);
			break;
		case COMMANDS::Set_Beta:
			SetBetaCommand(*packetptr);
			break;
		case COMMANDS::Reset_Orientation:
			ResetOrientationCommand(*packetptr);
			break;
		case COMMANDS::Reset_Localization:
			ResetLocalizationCommand(*packetptr);
			break;
		case COMMANDS::Enter_USBMode:
			EnterUSBModeCommand(*packetptr);
			break;
		case COMMANDS::Enter_Groundstation:
			EnterGroundstationCommand(*packetptr);
			break;
		case COMMANDS::Enter_Countdown:
			EnterCountdownCommand(*packetptr);
			break;
		case COMMANDS::Enter_Flight:
			EnterFlightCommand(*packetptr);
			break;
		case COMMANDS::Enter_Recovery:
			EnterRecoveryCommand(*packetptr);
			break;
		case COMMANDS::Exit_USBMode:
			ExitUSBModeCommand(*packetptr);
			break;
		case COMMANDS::Exit_to_USBMode:
			ExitUSBModeCommand(*packetptr);
			break;
		case COMMANDS::Set_Throttle:
			SetThrottleCommand(*packetptr);
			break;
		case COMMANDS::Engine_Info:
			EngineInfoCommand(*packetptr);
			break;
		case COMMANDS::Pyro_info:
			PyroInfoCommand(*packetptr);
			break;
		case COMMANDS::Fire_pyro:
			FireInfoCommand(*packetptr);
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



void CommandHandler::LaunchCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->changeState(new Launch(_sm));
}

void CommandHandler::ResetCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_RECOVERY,SYSTEM_FLAG::STATE_GROUNDSTATION)){
		return;
	}
	_sm->changeState(new Preflight(_sm));
}

void CommandHandler::AbortCommand(const  RnpPacketSerialized& packet) 
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

void CommandHandler::SetHomeCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	
}

void CommandHandler::StartLoggingCommand(const RnpPacketSerialized& packet) 
{
	SimpleCommandPacket commandpacket(packet);
	_sm->logcontroller.startLogging((LOG_TYPE)commandpacket.arg);
}

void CommandHandler::StopLoggingCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	SimpleCommandPacket commandpacket(packet);
	_sm->logcontroller.stopLogging((LOG_TYPE)commandpacket.arg);
}

void CommandHandler::TelemetryCommand(const RnpPacketSerialized& packet) 
{
	SimpleCommandPacket commandpacket(packet);

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

void CommandHandler::ClearFlashCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::STATE_GROUNDSTATION,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->storagecontroller.erase(STORAGE_DEVICE::FLASH);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete

}

void CommandHandler::ClearSDCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::STATE_GROUNDSTATION,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->storagecontroller.erase(STORAGE_DEVICE::MICROSD);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::PlaySongCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	SimpleCommandPacket commandpacket(packet);
	_sm->tunezhandler.play_by_idx(commandpacket.arg);
}

void CommandHandler::SkipSongCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->tunezhandler.skip();
}

void CommandHandler::ClearSongQueueCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->tunezhandler.clear();
}

void CommandHandler::ResetOrientationCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
}

void CommandHandler::ResetLocalizationCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->estimator.setup();
}

void CommandHandler::SetBetaCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	SimpleCommandPacket commandpacket(packet);
	float beta = ((float)commandpacket.arg) / 100.0;
	_sm->estimator.changeBeta(beta);
}

void CommandHandler::CalibrateAccelGyroBiasCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->sensors.imu.calibrateAccelGyroBias(true);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::CalibrateMagBiasCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->sensors.imu.calibrateMagBias(true);
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::CalibrateMagFullCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	//check packet type received
	if (packet.header.type != static_cast<uint8_t>(CommandPacket::TYPES::MAGCAL)){
		//incorrect packet type received do not deserialize
		return;
	}
	
	MagCalCommandPacket magcalpacket(packet);
	_sm->sensors.imu.calibrateMagFull(MagCalibrationParameters{magcalpacket.fieldMagnitude,
															   magcalpacket.inclination,
															   magcalpacket.declination,
															   magcalpacket.getA(),
															   magcalpacket.getB()});

}

void CommandHandler::EnterUSBModeCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
	_sm->systemstatus.new_message(SYSTEM_FLAG::DEBUG);
}

void CommandHandler::EnterGroundstationCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Groundstation(_sm));
}

void CommandHandler::EnterCountdownCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Launch(_sm));
}

void CommandHandler::EnterFlightCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Flight(_sm));
}

void CommandHandler::EnterRecoveryCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Recovery(_sm));
}

void CommandHandler::ExitUSBModeCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
	_sm->systemstatus.delete_message(SYSTEM_FLAG::DEBUG);
	_sm->changeState(new Preflight(_sm));
}

void CommandHandler::ExitToUSBModeCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new USBmode(_sm));
}

void CommandHandler::EngineInfoCommand(const RnpPacketSerialized& packet) 
{
	
}

void CommandHandler::SetThrottleCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_USBMODE)){
		return;
	}
}

void CommandHandler::PyroInfoCommand(const RnpPacketSerialized& packet) 
{
	
}

void CommandHandler::FireInfoCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_USBMODE)){
		return;
	}
};


