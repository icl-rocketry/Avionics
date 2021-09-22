
#include "groundstation.h"
#include "stateMachine.h"

#include "rnp_default_address.h"


#include "flags.h"

Groundstation::Groundstation(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_GROUNDSTATION)
{};

void Groundstation::initialise(){
    State::initialise();
    //load the groundstation routing table
    _sm->networkmanager.routingtable.clearTable();
    _sm->networkmanager.setAddress(static_cast<uint8_t>(DEFAULT_ADDRESS::GROUNDSTATION));

    _sm->networkmanager.routingtable.setRoute((uint8_t)DEFAULT_ADDRESS::ROCKET,Route{2,1,{}});
    _sm->networkmanager.routingtable.setRoute((uint8_t)DEFAULT_ADDRESS::DESKTOP,Route{1,1,{}});
    _sm->networkmanager.updateBaseTable(); // save the new base table

    _sm->networkmanager.enableAutoRouteGen(true); // enable route learning
    _sm->networkmanager.setNoRouteAction(RnpNetworkManager::NOROUTE_ACTION::BROADCAST,{1,2}); // enable broadcast over serial and radio
    
};

State* Groundstation::update(){
    
    return this;//loopy loop
};

void Groundstation::exitstate(){
    State::exitstate();
};