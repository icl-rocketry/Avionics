#pragma once

#include "pyro.h"
#include "rnp_networkmanager.h"
#include "Storage/systemstatus.h"
#include "rnp_packet.h"
#include <string>
#include <optional>
#include <memory>

class NetworkPyro:public Pyro{
    public:
        NetworkPyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,RnpNetworkManager& networkmanager,uint8_t nodeAddress);

        void check_continuity() override;
        void doStuff() override;
        
        /**
         * @brief Process a received network packet addressed to this pyro 
         * 
         * @param packet 
         */
        void networkCallback(std::unique_ptr<RnpPacketSerialized> packet);

    private:
        RnpNetworkManager& _networkmanager; // reference to network manager
        const uint8_t _nodeAddress; // network layer address

};

