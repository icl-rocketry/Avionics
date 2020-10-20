#include "radio.h"
#include "iface.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "LoRa.h"
#include "../packets.h"
#include "../../Logging/messages.h"

Radio::Radio(SPIClass* spi)
{
    _spi = spi; //pointer to spi object
};

void Radio::setup(){
    //setup lora moudule
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(*_spi);

    while (!LoRa.begin(LORA_REGION)){
        new_message(ERROR_LORA,"Lora setting up");
        delay(100);       
    };
    delete_message(ERROR_LORA);
    
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

void Radio::recieve_packet(uint8_t* rxpacket_ptr){
    int packetSize = LoRa.parsePacket();
    if (packetSize){ //check if theres data to read 
        LoRa.readBytes(rxpacket_ptr, packetSize);
    };
};

void Radio::update(){};