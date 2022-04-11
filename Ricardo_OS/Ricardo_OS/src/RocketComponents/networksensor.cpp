#include "networksensor.h"
#include "packets/nrcpackets.h"
#include "Commands/commandpacket.h"
#include <Arduino.h>

#include "Storage/logController.h"

NetworkSensor::~NetworkSensor(){};

void NetworkSensor::networkCallback(packetptr_t packetptr){
    if (packetptr->header.source != _address){
        return; // packet had incorrect data
    }
    switch (packetptr->header.type){
        case(static_cast<uint8_t>(NRCPacket::TYPES::NRC_STATE)):
        {
            NRCStatePacket nrcstatepacket(*packetptr); //deserialize state packet
            _state.lastNewStateUpdateTime = millis();
            _state.sensorValue = static_cast<float>(nrcstatepacket.value); // this is actually a float 
            _state.state = nrcstatepacket.state;
        }
    
    }

   
}

void NetworkSensor::updateState(){
    
    SimpleCommandPacket get_new_state(2,0); //command id = 2 for get state
    get_new_state.header.type = static_cast<uint8_t>(NRCPacket::TYPES::NRC_COMMAND);
    get_new_state.header.source = _networkmanager.getAddress();
    get_new_state.header.source_service = _sourceService;
    get_new_state.header.destination = _address;
    get_new_state.header.destination_service = _destinationService;

    _networkmanager.sendPacket(get_new_state);
    
    _state.lastNewStateRequestTime = millis();
}