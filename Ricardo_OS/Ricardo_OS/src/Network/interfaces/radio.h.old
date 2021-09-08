#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include "iface.h"
#include "SPI.h"
#include "../packets.h"
#include "Storage/systemstatus.h"

#include <memory>
#include <vector>
//class for lora type devices

enum class RADIO_INFO:uint8_t{
    RSSI = 0,
    SNR = 1
};
class Radio: public Iface{
    public:
        Radio(SPIClass* spi, SystemStatus* systemstatus,std::vector<std::unique_ptr<std::vector<uint8_t> > >& buf);
        void setup();
        /**
         * @brief send_packet places packet onto send buffer to be sent if radio isnt busy
         * This is to ensure that we can use the non-blocking send instead of waiting for radio to send each packet
         * This reduces latency in data logging
         * 
         * @param txpacket_ptr 
         * @param packet_size 
         */
        void send_packet(std::vector<uint8_t> &data);
        void update();
        

        std::vector<double> getRadioInfo();


    private:
        
        SPIClass* _spi; //pointer to spi class 
        SystemStatus* _systemstatus; //pointer to system status object  
        std::vector<std::unique_ptr<std::vector<uint8_t> > >& _packetBuffer;
        std::vector<std::vector<uint8_t> > _sendBuffer;

        bool _txDone;

        void getPacket();
        void checkSendBuffer();
        void checkTx();
        
  
};



#endif