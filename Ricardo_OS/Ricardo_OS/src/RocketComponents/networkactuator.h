#pragma once


#include "rocketcomponent.h"
#include "rocketcomponenttype.h"

#include "rocketactuator.h"

#include <rnp_networkmanager.h>

#include "stubs.h"

struct NetworkActuatorState : public RocketActuatorState{
    //cant think of any other members required here currently
};

class NetworkActuator: public RocketActuator{

    public:
        /**
         * @brief Construct a new Network Actuator object
         * 
         * @param id Id of the compoent given by managing object
         * @param componentType Type of component defined in rocketcomponent.h
         * @param logcontroller Reference to the logcontroller
         * @param address RNP node address for networked Component
         * @param sourceService Local Service which is incharge of the component
         * @param destinationService Destination Service on remote node
         *        Each Networked Component has multiple default services running
         *        Service 0 is reserved
         *        Service 1 is Network Management
         *        Service 2 is Command Service
         *        The rest of the services depend on the device. In the context of Networked Componets
         *        These extra services starting at 3 represent the component's channels e.g if we have
         *        the ematch board, service 3 would be the first pyro channel etc... The same applies for
         *        networked sensors
         * @param networkmanager reference to the network manager
         */
        NetworkActuator(uint8_t id,LogController& logcontroller,uint8_t address,uint8_t sourceService,uint8_t destinationService,RnpNetworkManager& networkmanager):
        RocketActuator(id,RocketComponentTypes::TYPES::NET_ACTUATOR,logcontroller),
        _address(address),
        _sourceService(sourceService),
        _destinationService(destinationService),
        _networkmanager(networkmanager)
        {};

        /**
         * @brief Actuates the Actuator with a given param
         * 
         * @param param - paramter to be sent with command, the meaning of which varies with the actuator type being addressed
         *              e.g if the actuator is a pyro, this will set the on time in ms whereas if this is a servo, it will be the
         *              angular position of the servo.
         */
        void execute(int32_t param) override;

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

        ~NetworkActuator(){};

    protected:
        const uint8_t _address;
        const uint8_t _sourceService;
        const uint8_t _destinationService; 

        RnpNetworkManager& _networkmanager;

        NetworkActuatorState _state;
    
};