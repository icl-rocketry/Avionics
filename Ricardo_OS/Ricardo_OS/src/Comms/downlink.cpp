#include "downlink.h"
#include "interfaces/radio.h"

Downlink::Downlink(stateMachine* sm){
    _sm = sm;
};


void Downlink::setup(){

};

void Downlink::send_data(uint8_t iface,uint32_t* data, size_t len){
    

};