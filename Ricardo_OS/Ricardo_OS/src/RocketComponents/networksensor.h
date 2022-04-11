#pragma once

#include "rocketcomponent.h"
#include "rocketcomponenttype.h"
#include "rocketsensor.h"

#include <rnp_networkmanager.h>

#include "Storage/logController.h"
struct NetworkSensorState: public RocketSensorState{
    //no extra members currently
};

class NetworkSensor:public RocketSensor{

    public:
        NetworkSensor(uint8_t id,LogController& logcontroller,uint8_t address,uint8_t sourceService,uint8_t destinationService,RnpNetworkManager& networkmanager):
        RocketSensor(id,RocketComponentTypes::TYPES::NET_SENSOR,logcontroller),
        _address(address),
        _sourceService(sourceService),
        _destinationService(destinationService),
        _networkmanager(networkmanager)
        {};

        void updateState() override;

        /**
         * @brief Returns the last received state from the actuator
         * 
         * @return const ComponentState* 
         */
        const RocketComponentState* getState() override {return &_state;};

        /**
         * @brief Network callback function used to process packets recevied by the network manager. This will usually be managed by
         *        a managing object.
         * 
         * @param packetptr 
         */
        void networkCallback(packetptr_t packetptr);

        ~NetworkSensor();

    protected:
        const uint8_t _address;
        const uint8_t _sourceService;
        const uint8_t _destinationService; 

        RnpNetworkManager& _networkmanager;
        NetworkSensorState _state;
    
};