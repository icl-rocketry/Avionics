#include "storageController.h"

#include <string>

StorageController::StorageController(stateMachine* sm):
_sm(sm)
{};

void StorageController::write(std::string path,std::string data,STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::ALL):{
            
            write(path,data,STORAGE_DEVICE::MICROSD);
            write(path,data,STORAGE_DEVICE::FLASH);

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