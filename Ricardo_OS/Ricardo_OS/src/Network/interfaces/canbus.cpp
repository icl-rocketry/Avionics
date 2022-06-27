#include "canbus.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <memory>
#include <vector>
#include <string>
#include <queue>

#include "rnp_interface.h"
#include "rnp_packet.h"

#include "driver/gpio.h"
#include "driver/can.h"

#include "ricardo_pins.h"



CanBus::CanBus(SystemStatus& systemstatus,LogController& logcontroller,uint8_t id,std::string name):
RnpInterface(id,name),
_systemstatus(systemstatus),
_logcontroller(logcontroller)
{
    _info.MTU = 256; //theoretical maximum is 2048 but this is very chonky
    _info.maxSendBufferElements = 10;
    _info.maxReceiveBufferElements = 10;
};

void CanBus::setup()
{
    if (can_driver_install(&can_general_config,&can_timing_config,&can_filter_config) != ESP_OK){
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can iface failed to install!");
        return;
    }
    if (can_start() != ESP_OK){
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can Iface failed to start!");
        return;
    }
    
}

void CanBus::sendPacket(RnpPacket& data){
    if ((data.header.size() + data.header.packet_len) > _info.MTU){
        _logcontroller.log("Packet Exceeds Can MTU!");
        return;
    }
    if (_sendBuffer.size() + 1 > _info.maxSendBufferElements){
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can Send Buffer Overflow!");
        _info.sendBufferOverflow = true;
        return;
    }

    std::vector<uint8_t> serializedPacket;
    data.serialize(serializedPacket);
    _sendBuffer.emplace(send_buffer_element_t{RnpCanIdentifier(data.header,generateCanPacketId()),serializedPacket});

    if (_info.sendBufferOverflow && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_CAN)){
        _systemstatus.delete_message(SYSTEM_FLAG::ERROR_CAN,"Can Send Buffer no longer overflowing!");
        _info.sendBufferOverflow = false;
    }

}


void CanBus::update(){

    processSendBuffer();
    processReceivedPackets();

    if (millis() - prevTime > cleanup_delta){
        cleanupReceiveBuffer();
        prevTime = millis();
    }

}

void CanBus::setAcceptanceFilter(uint32_t mask){};


uint8_t CanBus::generateCanPacketId()
{
    uint8_t can_packet_id = packet_counter % 0x1F;
    ++packet_counter;
    return can_packet_id;
}

void CanBus::processSendBuffer(){

    if (_sendBuffer.empty()){
        return;
    }

  
    const send_buffer_element_t& packet = _sendBuffer.front();
    const size_t data_size = packet.bytedata.size();
    const uint8_t curr_seg_id = packet.canidentifier.seg_id;

    uint8_t offset = curr_seg_id*8;
    uint8_t bytes_left = data_size - offset;

    can_message_t can_packet;
    can_packet.identifier = packet.canidentifier.getIdentifier();
    can_packet.flags = CAN_MSG_FLAG_EXTD;
    can_packet.data_length_code = (bytes_left > 8) ? 8 : bytes_left;

    std::memcpy(&can_packet.data,packet.bytedata.data() + offset,can_packet.data_length_code);

    int err = can_transmit(&can_packet,0);//non blocking send
    if (err != ESP_OK){
        if (err == ESP_ERR_TIMEOUT || err == ESP_FAIL){
            // can tx buffer full, dont increment seg_id and try to place on buffer next update
            return;
        }
        // proper error might be worth throwing here? -> future
        if (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_CAN)){
            _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can transmit failed with error code" + std::to_string(err));
        }
        return;
    }
    // check if we just sent the last segment of the rnp packet
    if (can_packet.data_length_code + offset == data_size){
        // all segments have been sent
        _sendBuffer.pop();
        return;
    }
    //increment seg_id noting the rollover to prevent repeating of 0
    //In this current implementation due to MTU restrictions, this should never happen, but is implemented
    //for sake of completeness.
    _sendBuffer.front().canidentifier.seg_id = (curr_seg_id >= 0xFF) ? 1 : curr_seg_id + 1; // increment seg_id
    return;

};

void CanBus::processReceivedPackets(){
    can_message_t can_packet;
    int err = can_receive(&can_packet,0);
    if (err != ESP_OK){
        if (err != ESP_ERR_TIMEOUT){
            if (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_CAN)){
                _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can Receive failed with error code" + std::to_string(err));
            }
        }
        return;
    }
    if (!(can_packet.flags & CAN_MSG_FLAG_EXTD)){
        _logcontroller.log("Bad Can Packet Type, Packet Dumped!");
        return;
    }
    //decode identifier
    const RnpCanIdentifier can_identifier(can_packet.identifier);
    const uint32_t can_packet_uid = RnpCanIdentifier::getCanPacketUID(can_packet.identifier);

    if (can_identifier.seg_id == 0) // marks the start of a new packet
    {
        //check if uid already exists in receive buffer
        if (_receiveBuffer.count(can_packet_uid)){
            // receive buffer already contains a matching uid implying that we will never receive
            // the rest of the previous packet with the matching key so remove the old packet. 
            // to ensure that someone hasnt messed up rollover of seg_id, we check the last seg_id so that we
            // are confident this is a new packet
            if (_receiveBuffer.at(can_packet_uid).seg_id == 0xFF){
                //we are now unsure wether this received packet marks a new packet or if it is the next segment in the sequence
                //so we dump the element in the receive buffer aswell as the new packet to avoid corrupted packets
                _receiveBuffer.erase(can_packet_uid);
                return;
            }
            _receiveBuffer.erase(can_packet_uid); //erase previous entry and start new packet
        }

        //construct new received packet buffer element
        //check if receiveBuffer has space to push back to
        if (_receiveBuffer.size() == _info.maxReceiveBufferElements){
            _info.receiveBufferOverflow = true;
            if (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_CAN)){
                _systemstatus.new_message(SYSTEM_FLAG::ERROR_CAN,"Can Receive Buffer Overflow" + std::to_string(err));
            }
            return;
        }

        _receiveBuffer.emplace(can_packet_uid,
                                receive_buffer_element_t{std::vector<uint8_t>(can_packet.data, can_packet.data + can_packet.data_length_code),
                                0,
                                0,
                                millis()});

        if (_info.receiveBufferOverflow && _systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_CAN)){
            _systemstatus.delete_message(SYSTEM_FLAG::ERROR_CAN,"Can Receive Buffer no longer overflowing!");
            _info.receiveBufferOverflow = false;
        }
        
        return;
    }
    //check if can packet uid is in the receive buffer
    if (!(_receiveBuffer.count(can_packet_uid))){
        return; // ignore received packet
    }
    //get correct receive buffer element
    receive_buffer_element_t& receive_buffer_element = _receiveBuffer.at(can_packet_uid);
    //check seg_id are sequential 
    if (((can_identifier.seg_id == receive_buffer_element.seg_id + 1)) || ((can_identifier.seg_id == 1) && (receive_buffer_element.seg_id == 0xFF)))
    {
        //append receive buffer element vector with can packet data
        const size_t bytedata_size = receive_buffer_element.bytedata.size();
        //resize vector
        receive_buffer_element.bytedata.resize(bytedata_size + can_packet.data_length_code);
        //copy new data
        std::memcpy(receive_buffer_element.bytedata.data(),&can_packet.data,can_packet.data_length_code);
        //update last time modified 
        receive_buffer_element.last_time_modified = millis();

        //check if the previous received can packet was the start of the rnp packet in which case we can deserialize the header to get expected length.
        if (receive_buffer_element.seg_id == 0 || receive_buffer_element.expected_size == 0)
        {
            if (receive_buffer_element.bytedata.size() >= RnpHeader::size())//make sure we have enough bytes to deserialize header
            {
                RnpHeader header(receive_buffer_element.bytedata);
                receive_buffer_element.expected_size = header.packet_len + RnpHeader::size();
                //reserve vector size to reduce more allocations
                receive_buffer_element.bytedata.reserve(receive_buffer_element.expected_size);
            }
        }
        //update seg_id
        receive_buffer_element.seg_id = can_identifier.seg_id;

        //check if we have fully received a packet
        if (receive_buffer_element.bytedata.size() == receive_buffer_element.expected_size){
            auto packet_ptr = std::make_unique<RnpPacketSerialized>(receive_buffer_element.bytedata);
            packet_ptr->header.src_iface = getID(); // update source interface id
            _packetBuffer->emplace(std::move(packet_ptr)); //push to network manager packet buffer
            //cleanup receive buffer
            _receiveBuffer.erase(can_packet_uid);
            return;
        }
    
    }
    else // can packet segment received out of order, delete recieve buffer element to prevent packet corruption. Maybe think of retransmit later
    {
        _receiveBuffer.erase(can_packet_uid);
        return;
    }

}

void CanBus::cleanupReceiveBuffer(){
    for (auto& [key, value] : _receiveBuffer){
        if (millis() - value.last_time_modified > receive_buffer_expiry){
            _receiveBuffer.erase(key);
        }
    }
}