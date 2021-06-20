#include "usb.h"

#include "global_config.h"
#include "iface.h"
#include "interfaces.h"

#include "Storage/systemstatus.h"

#include "../packets.h"

#include <memory>
#include <vector>
#include <array>


USB::USB(Stream* stream,SystemStatus* systemstatus) :
_stream(stream),
_systemstatus(systemstatus)
{
    _tmp_packet_data.resize(_packetHeader_size);
    
};

void USB::setup(){
};

void USB::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    //maybe check if we can write using serial.avalibalewrite not sure what action we would take 
    //if we couldnt write though

    _stream->write(data,size);
    

};

void USB::get_packet(std::vector<std::unique_ptr<std::vector<uint8_t>>> &buf){
    //return if stream object is null
    if (_stream == nullptr) return;   


    
    while (_stream->available() > 0){ // nonblocking??
        //find and process any and all packets
       
        
        //_firstByte = _stream->peek();
        _firstByte = _stream->read();
        
        if ((_firstByte == 0xAF && _stream->available() > 15)){
            
            if(!_incompletePacketReceived){
                //reset timeoutcounter
                
                
                _tmp_packet_data[0] = 0xAF; 
                _stream->readBytes(_tmp_packet_data.data()+1,_packetHeader_size-1);//this is a blocking operation
                
                //create packet header object to decode packet header and retrieve packet size
                PacketHeader packetheader = PacketHeader(_tmp_packet_data);
                //PacketHeader packetheader = PacketHeader();
                //get decoded packet length 
                _packet_len = packetheader.packet_len;
                _header_len = packetheader.header_len;
                _total_len = _packet_len+_header_len;
                

            };
            
            if (_packet_len > _stream->available()){
                //we dont have the full packet to read 
                _incompletePacketReceived = false; //dump the packet
                _systemstatus->new_message(system_flag::ERROR_SERIAL,"Packet Dumped");
                
                
            }else{
                //only a single packet to read or multiple packets to read so we will only the first packet
                _incompletePacketReceived = false;
                
                //create shared ptr with custom deleter
                //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[_total_len]);

                std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>();

                
                //deserialize packet header, modify source interface and reserialize into packet_ptr.
                Iface::updateSourceInterface(_tmp_packet_data,(*packet_ptr),Interface::USBSerial);
                
                //read bytes in stream buffer into the packet data array starting at the 8th index as header has been read out of stream buffer.
                //packet len has been decremented 8 as packet_len includes the packet header which is no longer in stream buffer
                
                //expand vector to total_len size
                (*packet_ptr).resize(_total_len);
                _stream->readBytes(((*packet_ptr).data() + _header_len), _packet_len); 

                //should add exceptioj checking here so we know if we have failed to properly read the data into the packet ptr
                buf.push_back(std::move(packet_ptr)); // add pointer to packet immediately to buffer                
                
            };
            
        }else{
            // read byte to clear byte in serial buffer
            _systemstatus->new_message(system_flag::ERROR_SERIAL,"Packet Dumped");
            
        };  
    }; 
/*
    while (_stream->available() > 0){
        //find and process any and all packets
       
        
        _firstByte = _stream->peek();
        
        if (_firstByte == 0xAF && _stream->available() > _packetHeader_size){ // ensure that there is atleast a packet header present
            
            _stream->readBytes(_tmp_packet_data.data(),_packetHeader_size); //read header from serial
            
            //create packet header object to decode packet header and retrieve packet size
            PacketHeader packetheader = PacketHeader(_tmp_packet_data);
            //PacketHeader packetheader = PacketHeader();
            //get decoded packet length 
            _packet_len = packetheader.packet_len;
            _header_len = packetheader.header_len;
            _total_len = _packet_len+_header_len;
            //_stream->write((uint8_t)0);
            if (_packet_len > _stream->available()){
                //we dont have the full packet to read 
                //dump the packet
                //_stream->write((uint8_t)1);
                _tmp_packet_data.clear();
                
                break;
            };
                
            std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>();
            (*packet_ptr).resize(_total_len);

            //update source interface
            packetheader.src_interface = static_cast<uint8_t>(Interface::USBSerial);
        
            //serialize packet header
            std::vector<uint8_t> modified_packet_header;
            packetheader.serialize(modified_packet_header);
            
            //copy data in modified_packet_header to packet container
            memcpy((*packet_ptr).data(),modified_packet_header.data(),_header_len);
            
           //read rest of packet from serial
           // _stream->write((uint8_t)3);
            _stream->readBytes(((*packet_ptr).data() + _header_len), _packet_len); 
           // _stream->write((uint8_t)4);
            //should add exceptioj checking here so we know if we have failed to properly read the data into the packet ptr
            buf.push_back(std::move(packet_ptr)); // add pointer to packet immediately to buffer                

            
        }else{
            // read byte to clear byte in serial buffer
            _stream->read(); 
        };  
    };   





    
    while (_stream->available() > 0){
        //find and process any and all packets
       
        
        _firstByte = _stream->peek();
        
        if ((_firstByte == 0xAF && _stream->available() > 15) || _incompletePacketReceived){
            
            if(!_incompletePacketReceived){
                //reset timeoutcounter
                _timeoutCounter = 0;
                
                
                _stream->readBytes(_tmp_packet_data.data(),_packetHeader_size);
                
                //create packet header object to decode packet header and retrieve packet size
                PacketHeader packetheader = PacketHeader(_tmp_packet_data);
                //PacketHeader packetheader = PacketHeader();
                //get decoded packet length 
                _packet_len = packetheader.packet_len;
                _header_len = packetheader.header_len;
                _total_len = _packet_len+_header_len;
                

            };
            
            if (_total_len > SERIAL_SIZE_RX){
                //This basically is a buffer overflow as the serial buffer wont be able to contain this size of packet
                //in theory this shouldn't happen unless someone does something wrong
                //TODO
                //handle oversized packets some how...
                //return nothing 
                //_stream->read(); // clear start byte so rest of packet is flushed

            }else if (_packet_len > _stream->available()){
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
                
            }else{
                //only a single packet to read or multiple packets to read so we will only the first packet
                _incompletePacketReceived = false;
                
                //create shared ptr with custom deleter
                //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[_total_len]);

                std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>();
                (*packet_ptr).resize(_total_len);
                
                
                
                //deserialize packet header, modify source interface and reserialize.
                
                PacketHeader packetheader = PacketHeader(_tmp_packet_data);
                
                
                //update source interface
                packetheader.src_interface = static_cast<uint8_t>(Interface::USBSerial);
            
                //serialize packet header
                std::vector<uint8_t> modified_packet_header;
                packetheader.serialize(modified_packet_header);
               
                //copy data in modified_packet_header to packet container
                memcpy((*packet_ptr).data(),modified_packet_header.data(),_header_len);
                
                //read bytes in stream buffer into the packet data array starting at the 8th index as header has been read out of stream buffer.
                //packet len has been decremented 8 as packet_len includes the packet header which is no longer in stream buffer
                _stream->readBytes(((*packet_ptr).data() + _header_len), _packet_len); 
                
                //should add exceptioj checking here so we know if we have failed to properly read the data into the packet ptr
                buf.push_back(std::move(packet_ptr)); // add pointer to packet immediately to buffer                
                
            };
            
        }else{
            // read byte to clear byte in serial buffer
            _stream->read(); 
        };  
    };      
    */
  
}; 