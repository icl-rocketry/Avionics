//process commands stored in command buffer and execute

#pragma once



#include <vector>
#include <memory>
#include <functional>

#include "commands.h"
#include "rnp_packet.h"
#include "rnp_networkmanager.h"
#include "commandpacket.h"


class stateMachine;//forward declaration to prevent circular dependancy


class CommandHandler {
    public:
        CommandHandler(stateMachine* sm);
        
        std::function<void(std::unique_ptr<RnpPacketSerialized>)> getCallback();

        static constexpr uint8_t serviceID = static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND); // serivce ID for network manager

    private:
        stateMachine* _sm; //pointer to state machine

        void handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr);
        
        void LaunchCommand(const RnpPacketSerialized& packet);
        void ResetCommand(const RnpPacketSerialized& packet);
        void AbortCommand(const  RnpPacketSerialized& packet);
        void SetHomeCommand(const RnpPacketSerialized& packet);
        void StartLoggingCommand(const RnpPacketSerialized& packet);
        void StopLoggingCommand(const RnpPacketSerialized& packet);
        void TelemetryCommand(const RnpPacketSerialized& packet);
        void ClearFlashCommand(const RnpPacketSerialized& packet);
        void ClearSDCommand(const RnpPacketSerialized& packet);
        void PlaySongCommand(const RnpPacketSerialized& packet);
        void SkipSongCommand(const RnpPacketSerialized& packet);
        void ClearSongQueueCommand(const RnpPacketSerialized& packet);
        void ResetOrientationCommand(const RnpPacketSerialized& packet);
        void ResetLocalizationCommand(const RnpPacketSerialized& packet);
        void SetBetaCommand(const RnpPacketSerialized& packet);
        void CalibrateAccelGyroBiasCommand(const RnpPacketSerialized& packet);
        void CalibrateMagBiasCommand(const RnpPacketSerialized& packet);
        void CalibrateMagFullCommand(const RnpPacketSerialized& packet);
        void CalibrateBaroCommand(const RnpPacketSerialized& packet);
        void EnterUSBModeCommand(const RnpPacketSerialized& packet);
        void EnterGroundstationCommand(const RnpPacketSerialized& packet);
        void EnterCountdownCommand(const RnpPacketSerialized& packet);
        void EnterFlightCommand(const RnpPacketSerialized& packet);
        void EnterRecoveryCommand(const RnpPacketSerialized& packet);
        void ExitUSBModeCommand(const RnpPacketSerialized& packet);
        void ExitToUSBModeCommand(const RnpPacketSerialized& packet);
        void EngineInfoCommand(const RnpPacketSerialized& packet);
        void SetThrottleCommand(const RnpPacketSerialized& packet);
        void PyroInfoCommand(const RnpPacketSerialized& packet);
        void FireInfoCommand(const RnpPacketSerialized& packet);

};	
