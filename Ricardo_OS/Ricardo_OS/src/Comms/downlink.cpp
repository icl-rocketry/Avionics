#include "downlink.h"
#include "interfaces/radio.h"
#include "stateMachine.h"


Downlink::Downlink(stateMachine* sm){
    _sm = sm;
};


void Downlink::setup(){
    
};

void Downlink::update(){

};

void Downlink::send_data(uint8_t iface,uint8_t* data, size_t len){
    

};