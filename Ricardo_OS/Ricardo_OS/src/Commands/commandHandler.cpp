#include "commandHandler.h"

#include <vector>
#include <functional>
#include <memory>


#include "stateMachine.h"
#include "States/debug.h"
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

#include "TelemetryPacket.h"

#include "Sensors/mmc5983ma.h"
#include "Sensors/sensorStructs.h"


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
		case COMMANDS::Set_Home:
			SetHomeCommand(*packetptr);
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
		case COMMANDS::Calibrate_Mag_Full:
			CalibrateMagFullCommand(*packetptr);
			break;
		case COMMANDS::Calibrate_Baro:
			CalibrateBaroCommand(*packetptr);
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
		case COMMANDS::Enter_Debug:
			EnterDebugCommand(*packetptr);
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
		case COMMANDS::Exit_Debug:
			ExitDebugCommand(*packetptr);
			break;
		case COMMANDS::Exit_to_Debug:
			ExitDebugCommand(*packetptr);
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
		case COMMANDS::Free_Ram:
			FreeRamCommand(*packetptr);
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
	_sm->estimator.setHome(_sm->sensors.getData());
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
	
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

	auto raw_sensors = _sm->sensors.getData();
	auto estimator_state = _sm->estimator.getData();

	telemetry.header.type = static_cast<uint8_t>(CommandPacket::TYPES::TELEMETRY_RESPONSE);
	telemetry.header.source = _sm->networkmanager.getAddress();
	telemetry.header.source_service = serviceID;
	telemetry.header.destination = commandpacket.header.source;
	telemetry.header.destination_service = commandpacket.header.source_service;
	telemetry.header.uid = commandpacket.header.uid; 
	telemetry.system_time = millis();

	telemetry.pn = estimator_state.position(0);
	telemetry.pe = estimator_state.position(1);
	telemetry.pd = estimator_state.position(2);

	telemetry.vn = estimator_state.velocity(0);
	telemetry.ve = estimator_state.velocity(1);
	telemetry.vd = estimator_state.velocity(2);

	telemetry.an = estimator_state.acceleration(0);
	telemetry.ae = estimator_state.acceleration(1);
	telemetry.ad = estimator_state.acceleration(2);

	telemetry.roll = estimator_state.eulerAngles(0);
	telemetry.pitch = estimator_state.eulerAngles(1);
	telemetry.yaw =estimator_state.eulerAngles(2);

	telemetry.q0 = estimator_state.orientation.w();
	telemetry.q1 = estimator_state.orientation.x();
	telemetry.q2 =estimator_state.orientation.y();
	telemetry.q3 =estimator_state.orientation.z();

	telemetry.lat = raw_sensors.gps.lat;
	telemetry.lng = raw_sensors.gps.lng;
	telemetry.alt = raw_sensors.gps.alt;
	telemetry.sat = raw_sensors.gps.sat;

	telemetry.ax = raw_sensors.accelgyro.ax;
	telemetry.ay = raw_sensors.accelgyro.ay;
	telemetry.az = raw_sensors.accelgyro.az;

	telemetry.h_ax = raw_sensors.accel.ax;
	telemetry.h_ay = raw_sensors.accel.ay;
	telemetry.h_az = raw_sensors.accel.az;

	telemetry.gx = raw_sensors.accelgyro.gx;
	telemetry.gy = raw_sensors.accelgyro.gy;
	telemetry.gz = raw_sensors.accelgyro.gz;

	telemetry.mx = raw_sensors.mag.mx;
	telemetry.my = raw_sensors.mag.my;
	telemetry.mz = raw_sensors.mag.mz;

	telemetry.baro_temp = raw_sensors.baro.temp;
	telemetry.baro_press = raw_sensors.baro.press;
	telemetry.baro_alt = raw_sensors.baro.alt;

	telemetry.batt_voltage = raw_sensors.batt.volt;
	telemetry.batt_percent= raw_sensors.batt.percent;

	telemetry.launch_lat = estimator_state.gps_launch_lat;
	telemetry.launch_lng = estimator_state.gps_launch_long;
	telemetry.launch_alt = estimator_state.gps_launch_alt;

	telemetry.system_status = _sm->systemstatus.getStatus();
	

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
	_sm->estimator.resetOrientation();
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::ResetLocalizationCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->estimator.setup();
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
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
	_sm->sensors.calibrateAccelGyro();
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
	_sm->sensors.calibrateMag(MagCalibrationParameters{magcalpacket.fieldMagnitude,
													   magcalpacket.inclination,
													   magcalpacket.declination,
													   magcalpacket.getA(),
													   magcalpacket.getB()});
	_sm->tunezhandler.play(MelodyLibrary::confirmation); // play sound when complete
}

void CommandHandler::CalibrateBaroCommand(const RnpPacketSerialized& packet)
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT,SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->sensors.calibrateBaro();
	_sm->tunezhandler.play(MelodyLibrary::confirmation); //play sound when complete
}

void CommandHandler::EnterDebugCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_PREFLIGHT)){
		return;
	}
	_sm->changeState(new Debug(_sm));
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

void CommandHandler::ExitDebugCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Debug(_sm));
	_sm->systemstatus.delete_message(SYSTEM_FLAG::DEBUG);
	_sm->changeState(new Preflight(_sm));
}

void CommandHandler::ExitToDebugCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
		return;
	}
	_sm->changeState(new Debug(_sm));
}

void CommandHandler::EngineInfoCommand(const RnpPacketSerialized& packet) 
{
	
}

void CommandHandler::SetThrottleCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_DEBUG)){
		return;
	}
}

void CommandHandler::PyroInfoCommand(const RnpPacketSerialized& packet) 
{
	
}

void CommandHandler::FireInfoCommand(const RnpPacketSerialized& packet) 
{
	if(!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::STATE_DEBUG)){
		return;
	}
};

void CommandHandler::FreeRamCommand(const RnpPacketSerialized& packet)
{	
	//avliable in all states
	//returning as simple string packet for ease
	//currently only returning free ram
	MessagePacket_Base<serviceID,static_cast<uint8_t>(CommandPacket::TYPES::MESSAGE_RESPONSE)> message("FreeRam: " + std::to_string(esp_get_free_heap_size()));
	message.header.source_service = serviceID;
	message.header.destination_service = packet.header.source_service;
	message.header.source = packet.header.destination;
	message.header.destination = packet.header.source;
	message.header.uid = packet.header.uid;
	_sm->networkmanager.sendPacket(message);
	
}
