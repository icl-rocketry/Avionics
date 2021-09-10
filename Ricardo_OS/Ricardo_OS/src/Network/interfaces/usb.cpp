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


USB::USB(HardwareSerial& serial,SystemStatus& systemstatus,LogController& logcontroller,std::string name) :
RnpInterface((uint8_t)DEFAULT_INTERFACES::USBSERIAL),
_serial(serial),
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_name(name)
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
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_SERIAL,"Send Buffer Overflow!");
        ++_info.txerror;
        return;
    }
    std::vector<uint8_t> serializedData;
    data.serialize(serializedData); // serialize the packet 
    COBS::encode(serializedData,_sendBuffer); //encode the serialized data with COBS
    _sendBuffer.push_back(COBS::marker); // push marker to mark end of packet
    checkSendBuffer();
};

void USB::checkSendBuffer(){
    if (_sendBuffer.size() == 0){
        return;
    }
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
                }
            }
        }
    
}




     // if (_serial.available() > 0){
    //     uint8_t incomming = _serial.peek();
    //     if ((incomming == 0xAF) && (_serial.available() > RnpHeader::size())){

    //         _serial.readBytes(_receiveBuffer.data(),RnpHeader::size()); //get enough data to decode header
    //         RnpHeader header(_receiveBuffer); //decode header

    //         if (header.packet_len > _serial.available()){
    //             // currently not handling this

    //             return;
    //         }

            

    //     }else{
    //         _serial.read();
    //     }
    // }
 
    // while (_serial.available() > 0){ // nonblocking??
    //     //find and process any and all packets
    
    //     //_firstByte = _serial->peek();
    //     _firstByte = _serial->read();
        
    //     if ((_firstByte == 0xAF && _serial.available() > RnpHeader::size())){
            
    //         if(!_incompletePacketReceived){
    //             //reset timeoutcounter
                
                
    //             _tmp_packet_data[0] = 0xAF; 
    //             _serial.readBytes(_tmp_packet_data.data()+1,_packetHeader_size-1);//this is a blocking operation
                
    //             //create packet header object to decode packet header and retrieve packet size
    //             PacketHeader packetheader = PacketHeader(_tmp_packet_data);
    //             //PacketHeader packetheader = PacketHeader();
    //             //get decoded packet length 
    //             _packet_len = packetheader.packet_len;
    //             _header_len = packetheader.header_len;
    //             _total_len = _packet_len+_header_len;
                

    //         };
            
    //         if (_packet_len > _serial->available()){
    //             //we dont have the full packet to read 
    //             _incompletePacketReceived = false; //dump the packet
    //             _systemstatus->new_message(SYSTEM_FLAG::ERROR_SERIAL,"Packet Dumped");
    //             //Serial.println('dump');
                
                
    //         }else{
    //             //only a single packet to read or multiple packets to read so we will only the first packet
    //             _incompletePacketReceived = false;
                
    //             //create shared ptr with custom deleter
    //             //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[_total_len]);

    //             std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>();

                
    //             //deserialize packet header, modify source interface and reserialize into packet_ptr.
    //             Iface::updateSourceInterface(_tmp_packet_data,(*packet_ptr),INTERFACE::USBSerial);
                
    //             //read bytes in stream buffer into the packet data array starting at the 8th index as header has been read out of stream buffer.
    //             //packet len has been decremented 8 as packet_len includes the packet header which is no longer in stream buffer
                
    //             //expand vector to total_len size
    //             (*packet_ptr).resize(_total_len);
    //             _serial->readBytes(((*packet_ptr).data() + _header_len), _packet_len); 

    //             //should add exceptioj checking here so we know if we have failed to properly read the data into the packet ptr
    //             // for (int i = 0; i<(*packet_ptr).size(); i++){
    //             //     Serial.print((*packet_ptr).at(i));
    //             // }
    //             _packetBuffer.push_back(std::move(packet_ptr)); // add pointer to packet immediately to buffer                

                
    //         };
            
    //     }else{
    //         // read byte to clear byte in serial buffer
    //         _systemstatus->new_message(SYSTEM_FLAG::ERROR_SERIAL,"Packet Dumped");
            
    //     };  
    // };