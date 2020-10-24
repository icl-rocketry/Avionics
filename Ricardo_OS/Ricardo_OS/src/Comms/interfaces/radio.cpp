#include "radio.h"
#include "iface.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "LoRa.h"
#include "../packets.h"

#include "Logging/systemstatus.h"
#include "flags.h"

Radio::Radio(SPIClass* spi,SystemStatus* systemstatus)
{
    _spi = spi; //pointer to spi object
    _systemstatus = systemstatus;
};

void Radio::setup(){
    //setup lora moudule
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(*_spi);

    while (!LoRa.begin(LORA_REGION)){
        _systemstatus->new_message(system_flag::ERROR_LORA,"Lora setting up");
        delay(100);       
    };
    _systemstatus->delete_message(system_flag::ERROR_LORA);
    
    LoRa.setSyncWord(LORA_SYNC_WORD);

};

//example usage send_packet((uint8_t*)&packet, sizeof(packet));

void Radio::send_packet(uint8_t* txpacket_ptr, size_t packet_len){
    if(LoRa.beginPacket()){
        LoRa.write(txpacket_ptr, packet_len);
        LoRa.endPacket();
    }else{
        //radio busy or some awful error
    };
};


void Radio::update(){
    int packetSize = LoRa.parsePacket();
    if (packetSize){ //check if theres data to read 
        uint8_t* tempdata; //how are we going to return data 
        LoRa.readBytes(tempdata, packetSize);
    };

};