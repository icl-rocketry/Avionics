//process commands stored in command buffer and execute

#pragma once



#include <vector>
#include <memory>
#include <functional>

#include "commands.h"
#include "rnp_packet.h"
#include "commandpacket.h"


class stateMachine;//forward declaration to prevent circular dependancy

class CommandHandler {
    public:
        CommandHandler(stateMachine* sm);
        
        std::function<void(std::unique_ptr<RnpPacketSerialized>)> getCallback();

        static constexpr uint8_t serviceID = 2; // serivce ID for network manager

    private:
        stateMachine* _sm; //pointer to state machine

        void handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr);
        
        void LaunchCommand(const CommandPacket &commandpacket);
        void ResetCommand(const CommandPacket &commandpacket);
        void AbortCommand(const  CommandPacket &commandpacket);
        void SetHomeCommand(const CommandPacket &commandpacket);
        void StartLoggingCommand(const CommandPacket &commandpacket);
        void StopLoggingCommand(const CommandPacket &commandpacket);
        void TelemetryCommand(const CommandPacket &commandpacket);
        void ClearFlashCommand(const CommandPacket &commandpacket);
        void ClearSDCommand(const CommandPacket &commandpacket);
        void PlaySongCommand(const CommandPacket &commandpacket);
        void SkipSongCommand(const CommandPacket &commandpacket);
        void ClearSongQueueCommand(const CommandPacket &commandpacket);
        void ResetOrientationCommand(const CommandPacket &commandpacket);
        void ResetLocalizationCommand(const CommandPacket &commandpacket);
        void SetBetaCommand(const CommandPacket &commandpacket);
        void CalibrateAccelGyroBiasCommand(const CommandPacket &commandpacket);
        void CalibrateMagBiasCommand(const CommandPacket &commandpacket);
        void EnterUSBModeCommand(const CommandPacket &commandpacket);
        void EnterGroundstationCommand(const CommandPacket &commandpacket);
        void EnterCountdownCommand(const CommandPacket &commandpacket);
        void EnterFlightCommand(const CommandPacket &commandpacket);
        void EnterRecoveryCommand(const CommandPacket &commandpacket);
        void ExitUSBModeCommand(const CommandPacket &commandpacket);
        void ExitToUSBModeCommand(const CommandPacket &commandpacket);
        void EngineInfoCommand(const CommandPacket &commandpacket);
        void SetThrottleCommand(const CommandPacket &commandpacket);
        void PyroInfoCommand(const CommandPacket &commandpacket);
        void FireInfoCommand(const CommandPacket &commandpacket);

};	
