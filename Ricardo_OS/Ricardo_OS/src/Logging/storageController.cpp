#include "storageController.h"

#include "microSD.h"
#include "flash.h"
#include <string>

void StorageController::write(std::string path,std::string data,STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::ALL):{
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

void StorageController::read(std::string path,STORAGE_DEVICE device)){
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