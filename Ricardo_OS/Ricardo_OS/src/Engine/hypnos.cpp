#include "hypnos.h"

Hypnos::Hypnos(uint8_t id,JsonObjectConst engineConfig,addNetworkCallbackF_t addNetworkCallbackF,RnpNetworkManager& networkmanager,uint8_t handlerServiceID,LogController& logcontroller):
Engine(id,engineConfig,addNetworkCallbackF,networkmanager,handlerServiceID,logcontroller)
{};


void Hypnos::execute(int32_t func){
    switch(func){
        case 2:
        {
            // Kiran is a poo
            vent();
            break;
        }
        default:
        {
            Engine::execute(func);
            break;
        }
    }
}

void Hypnos::vent() {
    throw std::logic_error("Vent not implemented");
}