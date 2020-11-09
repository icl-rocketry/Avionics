#include "usb.h"

#include "config.h"
#include "iface.h"

#include "Logging/systemstatus.h"

#include "../packets.h"


USB::USB(Stream* stream,SystemStatus* systemstatus) 
{
    _stream = stream;
    _systemstatus = systemstatus;
};

void USB::setup(){

};

void USB::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    //maybe check if we can write using serial.avalibalewrite not sure what action we would take 
    //if we couldnt write though
    Serial.write(data,size);
};

uint8_t* USB::get_packet(){
    //return if stream object is null
    if (_stream == nullptr) return nullptr;   

    while (_stream->available() > 0){
        //find and process any and all packets

        
        _firstByte = _stream->peek();
        
        if (_firstByte == 0xAF || _incompletePacketReceived){

            if(!_incompletePacketReceived){
                //reset timeoutcounter
                _timeoutCounter = 0;
                //read first bytes of stream to get packet header data into next elements in array
                _stream->readBytes(&_tmp_packet_data[0],_packetHeader_size);
                // delete previous instance of temporary packer header object - should be fine if _packetHeader_ptr is null
                delete _packetHeader_ptr; 
                //create new instance of a packet header and give the first bytes of incoming packet to decode
                _packetHeader_ptr = new PacketHeader(&_tmp_packet_data[0], _packetHeader_size);

            };

            if (_packetHeader_ptr->packet_len > SERIAL_SIZE_RX){
                //This basically is a buffer overflow as the serial buffer wont be able to contain this size of packet
                //in theory this shouldn't happen unless someone does something wrong
                //TODO
                //handle oversized packets some how...
                //return nothing flush buffer automatically
                return nullptr;

            }else if (_packetHeader_ptr->packet_len - _packetHeader_size > _stream->available()){
                //minus 8 to account for bytes read for header
                //we dont have the full packet to read 

                //increment timeoutcounter
                _timeoutCounter += 1;

                //return out of function so we can wait for new data without blocking - idk if this is a bad idea could cause a buffer overflow
                //potentially implement a timeout function so if the packet is formed wrong we dont get stuck here
                if (_timeoutCounter > MAX_SERIAL_TIMEOUT){
                    _incompletePacketReceived = false;
                }else{
                    _incompletePacketReceived = true;
                };

                return nullptr;
            }else{
                //only a single packet to read or multiple packets to read so we will only the first packet
                _incompletePacketReceived = false;

                uint8_t* packet_ptr = new uint8_t[_packetHeader_ptr->packet_len]; // Allocate a new chunk of memory for the packet
                
                //copy data in _tmp_packet_data to packet container
                for (int i = 0; i < _packetHeader_size-1; i++){
                    *(packet_ptr+i) = _tmp_packet_data[i];
                };
                //read bytes in stream buffer into the packet data array starting at the 8th index as header has been read out of stream buffer.
                //packet len has been decremented 8 as packet_len includes the packet header which is no longer in stream buffer
                _stream->readBytes((packet_ptr + _packetHeader_size), (_packetHeader_ptr->packet_len - _packetHeader_size)); 

                return packet_ptr;
            };
            
        }else{
            // read byte to clear byte in serial buffer
            _stream->read(); 
            return nullptr;
        };  
    };      

};   
