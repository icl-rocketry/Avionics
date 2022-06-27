#include "usb.h"

#include "global_config.h"
#include "flags.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include "rnp_interface.h"
#include "rnp_header.h"
#include "rnp_packet.h"

#include "cobs.h"

#include <memory>
#include <vector>
#include <array>

#include <HardwareSerial.h>


USB::USB(HardwareSerial& serial,SystemStatus& systemstatus,LogController& logcontroller,uint8_t id,std::string name) :
RnpInterface(id,name),
_serial(serial),
_systemstatus(systemstatus),
_logcontroller(logcontroller)
{
    _info.MTU = 256;
    _info.sendBufferSize = 1024;
    _info.receiveBufferSize = 1024;

    _sendBuffer.reserve(200); // reserve average packet size in the send buffer to reduce heapfrag
    _receiveBuffer.reserve(200);
    
};

void USB::setup()
{};


void USB::sendPacket(RnpPacket& data) 
{
    const size_t dataSize = data.header.size() + data.header.packet_len;
    if (dataSize > _info.MTU){
        _logcontroller.log("Packet Exceeds Serial MTU");
        ++_info.txerror;
        return;
    }
    if (dataSize + _sendBuffer.size() > _info.sendBufferSize){
        // not enough space
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_SERIAL,"USB Send Buffer Overflow!");
        ++_info.txerror;
        return;
    }
    std::vector<uint8_t> serializedData;
    data.serialize(serializedData); // serialize the packet 
    COBS::encode(serializedData,_sendBuffer); //encode the serialized data with COBS
    _sendBuffer.push_back(COBS::marker); // push marker to mark end of packet
    checkSendBuffer();
};

void USB::checkSendBuffer(){ // hmm this seems to be causing lag issues if a packet is split, i suspect the send buffer erase
    if (_sendBuffer.size() == 0){
        return;
    }
    // _serial.write(_sendBuffer.data(),_sendBuffer.size());
    // _sendBuffer.clear();
    const size_t numBytes = _serial.availableForWrite();
    if (numBytes < _sendBuffer.size()){
        _serial.write(_sendBuffer.data(),numBytes);
        _sendBuffer.erase(_sendBuffer.begin(),_sendBuffer.begin()+numBytes); // remove the sent data
    
    }else if (numBytes >= _sendBuffer.size()){ 
        _serial.write(_sendBuffer.data(),_sendBuffer.size());
        _sendBuffer.clear();
        //maybe shrink to fit vector?
    }
 };


void USB::update(){
    checkSendBuffer();
    getPackets();
}; 



void USB::getPackets(){
    
    while (_serial.available() > 0)
        {
            uint8_t incomming = _serial.read();
            if (_packetBuffer == nullptr){
                continue; //avoid processing overhead of COBS if we cant store the packet anywhere so read out the packet
            }

            if (incomming == COBS::marker) // marks the start and end of packet
            {
                std::vector<uint8_t> _decodedData; // decoded data will always be samller than encoded data

                size_t numDecoded = COBS::decode(_receiveBuffer,_decodedData);
                _decodedData.resize(numDecoded);

                auto packet_ptr = std::make_unique<RnpPacketSerialized>(_decodedData);
                packet_ptr->header.src_iface = getID();
                _packetBuffer->push(std::move(packet_ptr));

                _receiveBuffer.clear();
                _info.receiveBufferOverflow = false;
            }
            else
            {
                if (_receiveBuffer.size() < _info.receiveBufferSize)
                {
                    _receiveBuffer.push_back(incomming);
                }
                else
                {
                    //buffer overflow, dump buffer
                    _receiveBuffer.clear();
                    _info.receiveBufferOverflow = true;
                    _systemstatus.new_message(SYSTEM_FLAG::ERROR_SERIAL,"USB Receive Buffer Overflow!");
                }
            }
        }
    
}
