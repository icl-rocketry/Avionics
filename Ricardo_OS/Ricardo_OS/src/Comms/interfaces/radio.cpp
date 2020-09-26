#include "radio.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "LoRa.h"
#include "../../Logging/messages.h"



void Radio::setup(SPIClass& _spi){
    //setup lora moudule
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(_spi);

    while (!LoRa.begin(LORA_REGION)){
        new_message(ERROR_LORA,"Lora setting up");
        delay(100);       
    };
    delete_message(ERROR_LORA);
    
    LoRa.setSyncWord(LORA_SYNC_WORD);

};

void Radio::send_packet(uint8_t* txpacket, size_t packet_len){
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&txpacket, packet_len);
    LoRa.endPacket();
};

void Radio::recieve_packet(uint8_t* rxpacket){
    int packetSize = LoRa.parsePacket();
    if (packetSize){ //check if theres data to read 
        LoRa.readBytes((uint8_t*)&rxpacket, packetSize);
    };
};