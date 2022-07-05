
#include "groundstation.h"
#include "stateMachine.h"

#include "rnp_default_address.h"
#include <rnp_routingtable.h>


#include "flags.h"

Groundstation::Groundstation(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_GROUNDSTATION)
{};

void Groundstation::initialise(){
    State::initialise();
    //load the groundstation routing table
    RoutingTable groundRouting;
    groundRouting.setRoute((uint8_t)DEFAULT_ADDRESS::ROCKET,Route{2,1,{}});
    groundRouting.setRoute((uint8_t)DEFAULT_ADDRESS::GROUNDSTATION,Route{1,1,{}});

    _sm->networkmanager.setRoutingTable(groundRouting);
    _sm->networkmanager.updateBaseTable(); // save the new base table
    _sm->networkmanager.setAddress(static_cast<uint8_t>(DEFAULT_ADDRESS::GROUNDSTATION_GATEWAY));

    _sm->networkmanager.enableAutoRouteGen(true); // enable route learning
    _sm->networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1,2}); // enable broadcast over serial and radio
    
};

State* Groundstation::update(){
    
    return this;//loopy loop
};

void Groundstation::exitstate(){
    State::exitstate();
};