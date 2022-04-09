#include "networkactuator.h"

#include "packets/nrcpackets.h"
#include "../Commands/commandpacket.h"

#include "stubs.h"



void NetworkActuator::execute(int32_t param){
    

    SimpleCommandPacket get_new_state(3,static_cast<uint32_t>(param)); //command id = 3 for actuate
    get_new_state.header.type = static_cast<uint8_t>(NRCPacket::TYPES::NRC_COMMAND);
    get_new_state.header.source = _networkmanager.getAddress();
    get_new_state.header.source_service = _sourceService;
    get_new_state.header.destination = _address;
    get_new_state.header.destination_service = _destinationService;

    _networkmanager.sendPacket(get_new_state);

};

void NetworkActuator::updateState(){
    SimpleCommandPacket get_new_state(2,0); //command id = 2 for get state - no payload required
    get_new_state.header.type = static_cast<uint8_t>(NRCPacket::TYPES::NRC_COMMAND);
    get_new_state.header.source = _networkmanager.getAddress();
    get_new_state.header.source_service = _sourceService;
    get_new_state.header.destination = _address;
    get_new_state.header.destination_service = _destinationService;

    _networkmanager.sendPacket(get_new_state);
    _state.lastNewStateRequestTime = millis();
};

void NetworkActuator::networkCallback(packetptr_t packetptr){
    if (packetptr->header.source != _address){
        #ifdef _RICDEBUG
        std::cout<<"bad packet source"<<std::endl;
        #endif
        return; // packet had incorrect data
    }
    switch (packetptr->header.type){
        case(static_cast<uint8_t>(NRCPacket::TYPES::NRC_STATE)):
        {
            NRCStatePacket nrcstatepacket(*packetptr); //deserialize state packet
            _state.lastNewStateUpdateTime = millis();
            _state.currentValue = nrcstatepacket.value;
            _state.state = nrcstatepacket.state;
        }
    
    }

   
};



