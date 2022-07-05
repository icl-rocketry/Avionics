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

#include "Events/eventHandler.h"
#include "Deployment/deploymenthandler.h"
#include "Engine/enginehandler.h"
#include "Controller/controllerhandler.h"

#include "Sound/tunezHandler.h"

#include "Network/interfaces/usb.h"
#include "Network/interfaces/radio.h"
#include "Network/interfaces/canbus.h"

#include "rnp_networkmanager.h"
#include "rnp_default_address.h"

#include "SPI.h"
#include "Wire.h"

#include <ArduinoJson.h>



stateMachine::stateMachine() : 
    vspi(VSPI),
    hspi(HSPI),
    I2C(0),
    storagecontroller(this),
    logcontroller(&storagecontroller,networkmanager),
    systemstatus(&logcontroller),
    usbserial(Serial,systemstatus,logcontroller),
    radio(vspi,systemstatus,logcontroller),
    canbus(systemstatus,logcontroller,3),
    networkmanager(static_cast<uint8_t>(DEFAULT_ADDRESS::ROCKET),NODETYPE::HUB,true),
    commandhandler(this),
    sensors(hspi,I2C,systemstatus,logcontroller),
    estimator(systemstatus,logcontroller),
    deploymenthandler(networkmanager,deploymentHandlerServiceID,I2C,logcontroller),
    enginehandler(networkmanager,engineHandlerServiceID,logcontroller),
    controllerhandler(enginehandler,logcontroller),
    eventhandler(enginehandler,deploymenthandler,logcontroller)
{};


void stateMachine::initialise(State* initStatePtr) {

  //internal io initilization must happen here so io buses setup for sensor initialzation
  //intialize i2c interface
  I2C.begin(_SDA,_SCL,I2C_FREQUENCY);
  //initalize spi interface
  vspi.begin();
  vspi.setFrequency(1000000);
  vspi.setBitOrder(MSBFIRST);
  vspi.setDataMode(SPI_MODE0);

  hspi.begin();
  hspi.setFrequency(8000000);
  hspi.setBitOrder(MSBFIRST);
  hspi.setDataMode(SPI_MODE0);
  //setup cs pins
  //initialise output variables as output
  pinMode(LoraCs, OUTPUT);
  pinMode(ImuCs_1, OUTPUT);
  pinMode(ImuCs_2, OUTPUT);
  pinMode(BaroCs, OUTPUT);
  pinMode(MagCs, OUTPUT);
  pinMode(SdCs_1, OUTPUT);
  pinMode(SdCs_2, OUTPUT);

  //initialise outputs as high
  digitalWrite(LoraCs, HIGH);
  digitalWrite(ImuCs_1, HIGH);
  digitalWrite(ImuCs_2, HIGH);
  digitalWrite(BaroCs, HIGH);
  digitalWrite(MagCs, HIGH);
  digitalWrite(SdCs_1, HIGH);
  digitalWrite(SdCs_2, HIGH);
  //open serial port on usb interface
  Serial.begin(Serial_baud);
  Serial.setRxBufferSize(SERIAL_SIZE_RX);

  // call tunez handler setup first so we can provide startup tone and auditory cues asap
  tunezhandler.setup();

  //setup interfaces
  usbserial.setup();
  radio.setup();
  canbus.setup();

  //setup network manager so communication is running
  // add interfaces
  networkmanager.addInterface(&usbserial);
  networkmanager.addInterface(&radio);
  networkmanager.addInterface(&canbus);

  networkmanager.enableAutoRouteGen(false);
  networkmanager.setNoRouteAction(NOROUTE_ACTION::DUMP,{});

  // networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1}); // broadcasting back to usbserial for debugging

   // command handler callback
  networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),commandhandler.getCallback()); 

  //setup storage and logging so any erros encoutered can be logged
  storagecontroller.setup();
    
  logcontroller.setup();
  networkmanager.setLogCb([this](const std::string& message){return logcontroller.log(message);});

  // create config controller object
  ConfigController configcontroller(&storagecontroller,&logcontroller); 
  configcontroller.load(); // load configuration from sd card into ram

  //enumerate deployers engines controllers and events from config file
  try
  {
    deploymenthandler.setup(configcontroller.get()["Deployers"]);
    enginehandler.setup(configcontroller.get()["Engines"]);
    controllerhandler.setup(configcontroller.get()["Controllers"]);
    eventhandler.setup(configcontroller.get()["Events"]);
  }
  catch (const std::exception& e)
  {
    Serial.println("exception:");
    Serial.println(std::string(e.what()).c_str());
    //impelment panic handler to send crashed message to gc
    throw std::runtime_error("broke");
  }
  //register deployment and engine handler services
  networkmanager.registerService(deploymentHandlerServiceID,deploymenthandler.getThisNetworkCallback());  
  networkmanager.registerService(engineHandlerServiceID,enginehandler.getThisNetworkCallback());  


  // //sensors must be setup before estim ator
  sensors.setup(configcontroller.get());
  estimator.setup();
  //call setup state
  changeState(initStatePtr);
 
  
};

void stateMachine::update() {


  tunezhandler.update();

  //write logs to file 
  logcontroller.update();

  //request new sensor data
  sensors.update();
 
  //process updated sensor data
  estimator.update(sensors.getData());

  logcontroller.log(estimator.getData(),sensors.getData());// log new navigation solution and sensor output

  //check for new packets and process

  networkmanager.update();

  enginehandler.update();
  controllerhandler.update(estimator.getData());
  eventhandler.update(estimator.getData());

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

