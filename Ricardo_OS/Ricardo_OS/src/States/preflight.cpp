
#include "preflight.h"
#include "launch.h"
#include "stateMachine.h"

#include "rnp_default_address.h"
#include "rnp_routingtable.h"

#include "Sound/Melodies/melodyLibrary.h"

#include "flags.h"

Preflight::Preflight(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_PREFLIGHT)
{
    
};

void Preflight::initialise(){
    State::initialise();
    //load the rocket routing table

    RoutingTable flightRouting;
    flightRouting.setRoute((uint8_t)DEFAULT_ADDRESS::GROUNDSTATION,Route{2,1,{}});
    flightRouting.setRoute((uint8_t)DEFAULT_ADDRESS::DESKTOP,Route{2,2,{}});
    _sm->networkmanager.setRoutingTable(flightRouting);
    _sm->networkmanager.updateBaseTable(); // save the new base table

    _sm->networkmanager.setAddress(static_cast<uint8_t>(DEFAULT_ADDRESS::ROCKET));
    
    _sm->networkmanager.enableAutoRouteGen(false);
    _sm->networkmanager.setNoRouteAction(NOROUTE_ACTION::DUMP,{});
    

    _sm->tunezhandler.play(MelodyLibrary::miichannel,true);

};


State* Preflight::update(){
    return this;
};

void Preflight::exitstate(){
    State::exitstate();
    _sm->tunezhandler.clear();
};