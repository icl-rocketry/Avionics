#include "ricardo_pins.h"
#include "config.h"
#include "Arduino.h"

#include "setup.h"
#include "stateMachine.h"

#include "preflight.h"



Setup::Setup(stateMachine* sm) : State(sm){
    curr_stateID = 1; //? random id for now
}

void Setup::initialise(){
    State::initialise();
    //internal io initilization must happen here so io buses setup for sensor initialzation
        //intialize i2c interface
    _sm->I2C.begin(_SDA,_SCL,I2C_FREQUENCY);
        //initalize spi interface
        //todo find the lowest denominator for the speed of the spi bus.   
    _sm->vspi.setClockDivider(SPI_CLOCK_DIV8);
    _sm->vspi.begin();
        //setup cs pins
        //initialise output variables as output
    pinMode(LoraCs, OUTPUT);
    pinMode(ImuCs, OUTPUT);
    pinMode(BaroCs, OUTPUT);
    pinMode(MagCs, OUTPUT);
    pinMode(FlashCs, OUTPUT);
    pinMode(SdCs, OUTPUT);
        //initialise outputs as high
    digitalWrite(LoraCs, HIGH);
    digitalWrite(ImuCs, HIGH);
    digitalWrite(BaroCs, HIGH);
    digitalWrite(MagCs, HIGH);
    digitalWrite(FlashCs, HIGH);
    digitalWrite(SdCs, HIGH);
        //open serial port on usb interface
    Serial.begin(Serial_baud);

};


State* Setup::update(){
    //transtion to preflight state
    State* _preflight_ptr = new Preflight(_sm);
    return _preflight_ptr;
};

void Setup::exitstate(){
    State::exitstate();
};