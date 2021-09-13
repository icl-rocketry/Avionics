/* 
Inital State machine framework written by Daniele Bella <3
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"
#include <string>
#include <vector>
#include <functional>

#include "global_config.h"
#include "ricardo_pins.h"

#include "States/state.h"

#include "Storage/systemstatus.h"

#include "Storage/logController.h"
#include "Storage/storageController.h"
#include "Storage/configController.h"


#include "Sensors/estimator.h"
#include "Sensors/sensors.h"

#include "Sound/tunezHandler.h"

#include "Network/interfaces/usb.h"
#include "Network/interfaces/radio.h"

#include "rnp_networkmanager.h"
#include "rnp_default_address.h"

#include "SPI.h"
#include "Wire.h"





stateMachine::stateMachine() : 
    vspi(VSPI),
    I2C(0),
    storagecontroller(this),
    logcontroller(&storagecontroller),
    systemstatus(&logcontroller),
    usbserial(Serial,systemstatus,logcontroller),
    radio(vspi,systemstatus,logcontroller),
    networkmanager(static_cast<uint8_t>(DEFAULT_ADDRESS::ROCKET),NODETYPE::HUB,true),
    commandhandler(this),
    sensors(this),
    estimator(this)    
{};


void stateMachine::initialise(State* initStatePtr) {

  //internal io initilization must happen here so io buses setup for sensor initialzation
  //intialize i2c interface
  I2C.begin(_SDA,_SCL,I2C_FREQUENCY);
  //initalize spi interface
  vspi.begin();
  //_sm->vspi.setClockDivider(SPI_CLOCK_DIV2);
  vspi.setFrequency(1000000); // 10mhz
  vspi.setBitOrder(MSBFIRST);
  vspi.setDataMode(SPI_MODE0);
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
  Serial.setRxBufferSize(SERIAL_SIZE_RX);

  // call tunez handler setup first so we can provide startup tone and auditory cues asap
  tunezhandler.setup();
  
  //setup storage and logging so any erros encoutered can be logged
  storagecontroller.setup();
  logcontroller.setup();

  // create config controller object
  ConfigController configcontroller(&storagecontroller,&logcontroller); 
  configcontroller.load(); // load configuration from sd card into ram
  //setup interfaces
  usbserial.setup();
  radio.setup();


  //setup network manager so communication is running
  // add interfaces
  networkmanager.addInterface(&usbserial);
  networkmanager.addInterface(&radio);
  //load rt table
  networkmanager.enableAutoRouteGen(false);
  networkmanager.setNoRouteAction(RnpNetworkManager::NOROUTE_ACTION::DUMP,{});
  networkmanager.setLogCb([this](const std::string& message){return logcontroller.log(message);});
  networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),commandhandler.getCallback()); // register command handler callback

  //sensors must be setup before estimator
  sensors.setup();
  estimator.setup();

  //call setup state
  changeState(initStatePtr);
 
  
};

void stateMachine::update() {
  //call udpate on tunez handler

  tunezhandler.update();
  //write logs to file 
  logcontroller.update();
  //request new sensor data
  sensors.update();
  //process updated sensor data
  estimator.update();
  logcontroller.log(estimator.state,sensors.sensors_raw);// log new navigation solution and sensor output
  //check for new packets and process
  networkmanager.update();

  //call update on state after new information has been processed
  State* newStatePtr = _currStatePtr->update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
};

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  if (_currStatePtr != NULL){
    //if not null pointer call exitstate method
    _currStatePtr->exitstate();

  };

  delete _currStatePtr;
  
  _currStatePtr = newStatePtr;
  _currStatePtr->initialise();



};

