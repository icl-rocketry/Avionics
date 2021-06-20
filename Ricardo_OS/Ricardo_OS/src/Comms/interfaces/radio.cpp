#include "radio.h"
#include "iface.h"

#include "global_config.h"

#include "SPI.h"
#include "ricardo_pins.h"
#include "LoRa.h"
#include "../packets.h"

#include "Storage/systemstatus.h"
#include "flags.h"

#include <memory>
#include <Arduino.h>

#include "interfaces.h"

#include <vector>




Radio::Radio(SPIClass* spi,SystemStatus* systemstatus):
_spi(spi),
_systemstatus(systemstatus),
packetSize(0),
index(0)
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
    LoRa.enableCrc();
};



void Radio::send_packet(uint8_t* data, size_t packet_len){

    if(LoRa.beginPacket()){
        LoRa.write(data, packet_len);
        LoRa.endPacket();
    }else{
        //radio busy or some awful error
    };
    LoRa.receive();
};

void Radio::get_packet(std::vector<std::unique_ptr<std::vector<uint8_t>>> &buf){
    packetSize = LoRa.parsePacket();
    /*
    if(millis() - prev_time > 10){
        Serial.print(packetSize);
        Serial.print(" ");
        Serial.print(LoRa.available());
        Serial.print("\n");
        prev_time = millis();
    }*/

    if(packetSize){
        index = 0;
        packet.resize(packetSize);
        while(LoRa.available() || (index < (packetSize-1))){
            if (LoRa.available()){
                packet.at(index) = LoRa.read();
                index++;
            }
        }

        std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(packet);

        std::vector<uint8_t> modified_packet_header;
        //Iface::updateSourceInterface((*packet_ptr),modified_packet_header,Interface::LORA)



        PacketHeader packetheader = PacketHeader(*packet_ptr);
    
        //update source interface
        packetheader.src_interface = static_cast<uint8_t>(Interface::LORA);
        
        //serialize packet header
        
        
        packetheader.serialize(modified_packet_header);
        

        memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
/*
        Serial.print(" radio receive ");
        Serial.print(" ");
        Serial.print(packetSize);
        Serial.print(" ");
        Serial.print((*packet_ptr).size());
        Serial.print(" ");
        for (int i = 0; i< (*packet_ptr).size();i++){
            Serial.print((*packet_ptr)[i]);
            Serial.print(" ");
        }
        Serial.print("\n");*/

        buf.push_back(std::move(packet_ptr));//add packet ptr  to buffer

    }

/*
    if (!LoRa.parsePacket()){
        index = 0;
    }else{
        if (index == 0){
            packetSize = LoRa.parsePacket();
            packet.resize(packetSize);
            packet.at(index) = LoRa.read();
            index++;
        }else if (!LoRa.available() && (index == packetSize-1) ){
            std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(packet);

            PacketHeader packetheader = PacketHeader(*packet_ptr);
        
            //update source interface
            packetheader.src_interface = static_cast<uint8_t>(Interface::LORA);
            
            //serialize packet header
            std::vector<uint8_t> modified_packet_header;
            
            packetheader.serialize(modified_packet_header);
            

            memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
            

            buf.push_back(std::move(packet_ptr));//add packet ptr  to buffer

        

        }else if (LoRa.available()){
            packet.at(index) = LoRa.read();
            index++;
        }

    }
*/
/*
    
    if (LoRa.avaliable()){
        if (index == 0){
            packetSize = LoRa.parsePacket();
            packet.resize(packetSize);
            packet.at(index) = LoRa.read();
            index++;
        }else if (index == (packetSize - 1)){
            std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(packet);

            PacketHeader packetheader = PacketHeader(*packet_ptr);
        
            //update source interface
            packetheader.src_interface = static_cast<uint8_t>(Interface::LORA);
            
            //serialize packet header
            std::vector<uint8_t> modified_packet_header;
            
            packetheader.serialize(modified_packet_header);
            

            memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
            

            buf.push_back(std::move(packet_ptr));//add packet ptr  to buffer

        

        }else{
            packet.at(index) = LoRa.read();
            index++;
        }
    }*/
    /*
    packetSize = LoRa.parsePacket();

    if (packetSize && (LoRa.available() == packetSize)){ //check if theres data to read and there is at least a header present
        
        //create shared ptr with custom deleter
        //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[packetSize]); 
        
        std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>();
        
        (*packet_ptr).resize(packetSize);
        
        LoRa.readBytes((*packet_ptr).data(), packetSize); // Copy the received data into packet_received
        
        Serial.print(" radio receive ");
        Serial.print(" ");
        Serial.print(packetSize);
        Serial.print(" ");
        Serial.print((*packet_ptr).size());
        Serial.print(" ");
        for (int i = 0; i< (*packet_ptr).size();i++){
            Serial.print((*packet_ptr)[i]);
            Serial.print(" ");
        }

        //deserialize packet header, modify source interface and reserialize.
        PacketHeader packetheader = PacketHeader(*packet_ptr);
        
        //update source interface
        packetheader.src_interface = static_cast<uint8_t>(Interface::LORA);
        
        //serialize packet header
        std::vector<uint8_t> modified_packet_header;
        
        packetheader.serialize(modified_packet_header);
        

        memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
        

        buf.push_back(std::move(packet_ptr));//add packet ptr  to buffer
        

    }*/
    
}

int16_t Radio::get_rssi(){
    return LoRa.packetRssi();
}

float Radio::get_snr(){
    return LoRa.packetSnr();
}