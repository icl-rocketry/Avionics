#include "setup.h"
#include "preflight.h"

Setup::Setup(stateMachine* sm) : State(sm){
    _stateID = 1;// fill in number for demo purpose

}


State* Setup::update(){


    // test transition to preflight

    State* preflightstate_ptr = new Preflight(_sm);
    return preflightstate_ptr;
}