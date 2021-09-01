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




Radio::Radio(SPIClass* spi,SystemStatus* systemstatus,std::vector<std::unique_ptr<std::vector<uint8_t> > >& buf):
_spi(spi),
_systemstatus(systemstatus),
_packetBuffer(buf),
_txDone(true)
{
    _sendBuffer.reserve(5); // preallocation to reduce heap fragmentation
};

void Radio::setup(){
    //setup lora module
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(*_spi);

    while (!LoRa.begin(LORA_REGION)){
        _systemstatus->new_message(SYSTEM_FLAG::ERROR_LORA,"Lora setting up");
        delay(100);       
    };
    if (_systemstatus->flag_triggered(SYSTEM_FLAG::ERROR_LORA)){
        _systemstatus->delete_message(SYSTEM_FLAG::ERROR_LORA);
    }
    
    LoRa.setSyncWord(LORA_SYNC_WORD);
    LoRa.setSignalBandwidth(250e3);
    LoRa.setSpreadingFactor(7);
    LoRa.enableCrc();
};



void Radio::send_packet(std::vector<uint8_t> &data){

    _sendBuffer.push_back(data); 
};

void Radio::update(){
    getPacket();
    checkSendBuffer();
    checkTx();
};

void Radio::getPacket(){
    // check if radio is still transmitting
    if (!_txDone){  // this maybe able to be replaced wiht the begin packet method
        return;
    }

    int packetSize = LoRa.parsePacket(); // put radio back into single receive mode and check for packets

    if (packetSize){
        std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(packetSize);

        LoRa.readBytes((*packet_ptr).data(),packetSize); // read bytes into vector

        std::vector<uint8_t> modified_packet_header;
        //Iface::updateSourceInterface((*packet_ptr),modified_packet_header,INTERFACE::LORA)
        PacketHeader packetheader = PacketHeader(*packet_ptr);

        //update source interface
        packetheader.src_interface = static_cast<uint8_t>(INTERFACE::LORA);
        
        //serialize packet header
        
        
        packetheader.serialize(modified_packet_header);
        

        memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);

        _packetBuffer.push_back(std::move(packet_ptr));//add packet ptr  to buffer

    }
}

void Radio::checkSendBuffer(){

    if (!(_sendBuffer.size() > 0)){
        return; // exit if nothing in the buffer
    }

    // check if radio is busy, if it isnt then send next packe
    if(LoRa.beginPacket()){ 
        std::vector<uint8_t> packet = _sendBuffer.front();
        LoRa.write(packet.data(), packet.size());
        LoRa.endPacket(true); // asynchronous send 
        //delete front element of send buffer
        _sendBuffer.erase(_sendBuffer.begin());
        _txDone = false;
    }
    //LoRa.receive(); // place radio into receive mode
}

void Radio::checkTx(){
    if (_txDone){
        return;
    }
    if (!LoRa.isTransmitting()){
        _txDone = true;
    }
}


std::vector<double> Radio::getRadioInfo(){
    std::vector<double> info;
    info.resize(2);
    info.at((uint8_t)RADIO_INFO::RSSI) = LoRa.packetRssi();
    info.at((uint8_t)RADIO_INFO::SNR) = LoRa.packetSnr();
    return info;
}

