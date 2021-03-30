#include "radio.h"
#include "iface.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "LoRa.h"
#include "../packets.h"

#include "Logging/systemstatus.h"
#include "flags.h"

#include <memory>
#include <Arduino.h>

#include "interfaces.h"

#include <vector>




Radio::Radio(SPIClass* spi,SystemStatus* systemstatus):
_spi(spi),
_systemstatus(systemstatus)
{};

void Radio::setup(){
    //setup lora module
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(*_spi);

    while (!LoRa.begin(LORA_REGION)){
        _systemstatus->new_message(system_flag::ERROR_LORA,"Lora setting up");
        delay(100);       
    };
    if (_systemstatus->flag_triggered(system_flag::ERROR_LORA)){
        _systemstatus->delete_message(system_flag::ERROR_LORA);
    }
    
    LoRa.setSyncWord(LORA_SYNC_WORD);

};


void Radio::send_packet(uint8_t* data, size_t packet_len){
    if(LoRa.beginPacket()){
        LoRa.write(data, packet_len);
        LoRa.endPacket(true);
    }else{
        //radio busy or some awful error
    };
};


void Radio::get_packet(std::vector<std::shared_ptr<std::vector<uint8_t>>> &buf){
    int packetSize = LoRa.parsePacket();
    
    if (packetSize){ //check if theres data to read 
    
        //create shared ptr with custom deleter
        //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[packetSize]); 

        std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();

        (*packet_ptr).resize(packetSize);

        LoRa.readBytes((*packet_ptr).data(), packetSize); // Copy the received data into packet_received

        //deserialize packet header, modify source interface and reserialize.
        PacketHeader packetheader = PacketHeader(*packet_ptr);
        //update source interface
        packetheader.src_interface = static_cast<uint8_t>(Interface::LORA);
        //serialize packet header
        std::vector<uint8_t> modified_packet_header;
        packetheader.serialize(modified_packet_header);

        memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
        
        buf.push_back(packet_ptr);//add packet ptr  to buffer

    };
    
};


