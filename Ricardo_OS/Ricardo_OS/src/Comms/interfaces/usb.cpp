#include "usb.h"

#include "config.h"
#include "iface.h"

#include "Logging/systemstatus.h"

#include "packets.h"


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
    if (_stream == nullptr) return;   

    while (_stream->available() > 0){
        //find and process any and all packets
        if (!_incompletePacketReceived){
            _firstByte = _stream->read();
        };
        if (_firstByte == 0xAF || _incompletePacketReceived){
            //peek length of packet this number will be 1 greater than the stream buffer size as we have already read the first byte out to get the packet start
            uint8_t packet_len = _stream->peek(); //this only works if packet len is 8 bit or max size of 255. Currently not an issue but could be annoying if we create larger packets although then the limitation is the lora library


            if (packet_len > SERIAL_SIZE_RX){
                //This basically is a buffer overflow as the serial buffer wont be able to contain this size of packet
                //in theory this shouldnt happen unless someone does something wrong
                //TODO
                //handle oversized packets some how...

            }else if (packet_len-1>_stream->available()){
                //we dont have the full packet to read 
                _incompletePacketReceived = true;
                //return out of function so we can wait for new data without blocking - idk if this is a bad idea could cause a buffer overflow

                return nullptr;
            }else{
                //only a single packet to read or multiple packets to read so we will only the first packet
                _incompletePacketReceived = false;

                uint8_t* packet_ptr = new uint8_t[packet_len]; // Allocate a new chunk of memory for the packet
                *(packet_ptr+0) = 0xAF; //set zeroth element of data array to start byte of new packet for the packet decoder

                //read bytes in stream buffer into the packet data array starting at the first index as 0xAF has been read out of stream buffer.
                //packet len has been decremented 1 as packet_len includes the packet start byte which is no longer in stream buffer
                _stream->readBytes((packet_ptr+1), (packet_len-1)); 

                return packet_ptr;
            };
            
            
        }else{
            //packet start not present so continue reading data out of stream
            //dont return here so we can get rid of unwanted stuff
        };


};   
