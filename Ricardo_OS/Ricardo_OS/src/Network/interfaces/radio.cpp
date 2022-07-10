#include "radio.h"

#include "global_config.h"

#include <SPI.h>
#include "ricardo_pins.h"
#include <LoRa.h>

#include "Storage/systemstatus.h"
#include "flags.h"

#include <memory>
#include <vector>

#include "rnp_interface.h"
#include "rnp_packet.h"

Radio::Radio(SPIClass& spi, SystemStatus& systemstatus,LogController& logcontroller,uint8_t id ,std::string name):
RnpInterface(id,name),
_spi(spi),
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_currentSendBufferSize(0),
_txDone(true)
{
    _info.MTU = 256;
    _info.sendBufferSize = 2048;
};


void Radio::setup(){
    //setup lora module
    LoRa.setPins(LoraCs,LoraReset,LoraInt);
    LoRa.setSPI(_spi);


    if (!LoRa.begin(LORA_REGION)){
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_LORA,"Lora setting up");      
    };
    if (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_LORA)){
        _systemstatus.delete_message(SYSTEM_FLAG::ERROR_LORA);
    }
    
    LoRa.setSyncWord(LORA_SYNC_WORD);
    LoRa.setSignalBandwidth(250e3);
    LoRa.setSpreadingFactor(7);
    LoRa.enableCrc();
};



void Radio::sendPacket(RnpPacket& data)
{
    const size_t dataSize = data.header.size() + data.header.packet_len;
    if (dataSize > _info.MTU){ // will implement packet segmentation here at a later data
        _logcontroller.log("Packet Exceeds Radio MTU");
        ++_info.txerror;
        return;
    }
    if (dataSize + _currentSendBufferSize > _info.sendBufferSize){
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_LORA," LoRa Send Buffer Overflow!");
        ++_info.txerror;
        _info.sendBufferOverflow = true;
        return;
    }

    std::vector<uint8_t> serializedPacket;
    data.serialize(serializedPacket);
    _sendBuffer.push(serializedPacket); // add to send buffer
    _info.sendBufferOverflow = false;
    _currentSendBufferSize += dataSize;
    checkSendBuffer(); // see if we can send 
    

}

void Radio::update(){
    getPacket();
    checkSendBuffer();
    checkTx();
}


void Radio::getPacket(){
    // check if radio is still transmitting
    if (!_txDone){  // this maybe able to be replaced wiht the begin packet method
        return;
    }

    int packetSize = LoRa.parsePacket(); // put radio back into single receive mode and check for packets

    if (packetSize){

        std::vector<uint8_t> data(packetSize);
        LoRa.readBytes(data.data(),packetSize);

        if (_packetBuffer == nullptr){
            return;
        }

        auto packet_ptr = std::make_unique<RnpPacketSerialized>(data);
        //update source interface
        packet_ptr->header.src_iface = getID();
        _packetBuffer->push(std::move(packet_ptr));//add packet ptr  to buffer

    }
}

void Radio::checkSendBuffer(){
    if (_sendBuffer.size() == 0){
        return; // exit if nothing in the buffer
    }
    
    // check if radio is busy, if it isnt then send next packet
    size_t bytes_written = send(_sendBuffer.front());
    if (bytes_written){ // if we succesfully send packet
        _sendBuffer.pop(); //remove packet from buffer
        _currentSendBufferSize -= bytes_written;
    }
}

size_t Radio::send(std::vector<uint8_t> &data){
    if (LoRa.beginPacket()){
        LoRa.write(data.data(), data.size());
        LoRa.endPacket(true); // asynchronous send 
        _txDone = false;
        return data.size();
    }else{
        return 0;
    }
}

void Radio::checkTx(){
    if (_txDone){
        return;
    }
    if (!LoRa.isTransmitting()){
        _txDone = true;
    }
}


const RnpInterfaceInfo* Radio::getInfo()
{
     _info.rssi = LoRa.rssi();
     _info.snr = LoRa.packetSnr();  
     _info.freqError = LoRa.packetFrequencyError();
     return &_info;
};
