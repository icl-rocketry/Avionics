#include "storageController.h"
#include "stateMachine.h"
#include "ricardo_pins.h"
#include <Arduino.h>
#include <string>
#include <SPI.h>
#include <SdFat.h>

StorageController::StorageController(stateMachine* sm):
_sm(sm),
microsd(&(sm->vspi)),
flashTransport(FlashCs,&(sm->vspi)),
flash(&flashTransport)
{};

bool StorageController::setup(){
 
    
    if(!microsd.begin(SdCs,SD_SCK_MHZ(10))){
        
    }
    
    if (!microsd.exists("/Logs")){
        microsd.mkdir("/Logs"); // create logs directory
    }
    
    /*
    if(!flash.begin(&flash_config)){

    }
    if(!flash_fatfs.begin(&flash)){

    }*/

    

};


void StorageController::write(std::string &path,std::string &data,STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::ALL):{
            
            write(path,data,STORAGE_DEVICE::MICROSD);
            write(path,data,STORAGE_DEVICE::FLASH);

            break;
        }
        case(STORAGE_DEVICE::MICROSD):{
            file = microsd.open(path.c_str(), (O_WRITE | O_CREAT | O_AT_END));
            if (file){
                //check if file is okay
                file.write(data.c_str());
            }
            file.close();//close the file
            break;
        }
        case(STORAGE_DEVICE::FLASH):{


            break;
        }
        default:{
            //do nothing
            break;
        }
    }
}

void StorageController::read(std::string path,STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::ALL):{
            //illegal option - do nothing
            break;
        }
        case(STORAGE_DEVICE::MICROSD):{
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            break;
        }
        default:{
            //do nothing
            break;
        }
    }
}