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
 
    microsd.begin(SdCs,SD_SCK_MHZ(10));
    
    return true;
    /*
    if(!microsd.begin(SdCs,SD_SCK_MHZ(10))){
        
    }*/
    
    /*
    if (!microsd.exists("/Logs")){
        microsd.mkdir("/Logs"); // create logs directory
    }*/
    
    /*
    if(!flash.begin(&flash_config)){

    }
    if(!flash_fatfs.begin(&flash)){

    }*/

    

};

std::string StorageController::updateDirectoryName(std::string input_directory,STORAGE_DEVICE device){
    
}


bool StorageController::ls(std::string path,std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device){
    switch(device){
        case STORAGE_DEVICE::MICROSD:{
            microsd.chvol();//change vol to microsd
            file = microsd.open(path.c_str()); // open the path supplied
            break;
        }
        case STORAGE_DEVICE::FLASH:{
            flash_fatfs.chvol();//change vol to 
            file = flash_fatfs.open(path.c_str()); // open the path supplied
            break;
        }
        default:{
            return false; // return nothing 
        }
    }
    
    if((!file) || (!file.isDirectory())){
        //path invalid so return
        return false;
    }
    File child = file.openNextFile();//open next file in directory

    while(child){ // while child file is valid
        directory_element_t entry; // create new entry for directory list
        char filename[64];
        child.getName(filename, sizeof(filename)); // get file name
        entry.name = std::string{filename}; // convert char array to string and pass into entry structure
        //check if child is directory
        if (child.isDirectory()){
            entry.type = FILE_TYPE::DIRECTORY;
        }else{
            entry.type = FILE_TYPE::FILE;
        }
        entry.size = child.size(); // get file size

        directory_structure.push_back(entry); // add entry to vector
        child = file.openNextFile(); // open next file

    }  
    return true;
};

void StorageController::printDirectory(std::string path,STORAGE_DEVICE device){ // function just to check functionality of ls
    std::vector<directory_element_t> directory;
    ls(path,directory,device);
        for (int i = 0;i < directory.size(); i++){
            Serial.print("Name: ");
            Serial.print(directory[i].name.c_str());
            Serial.print(" Type: ");
            Serial.print((uint8_t)directory[i].type);
            Serial.print(" Size: ");
            Serial.print(directory[i].size);
            Serial.print("\n");
    }
}
        
        
bool StorageController::ls(std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device){
    std::string path = "/";
    return ls(path,directory_structure, device);
};


void StorageController::write(std::string &path,std::string &data,STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::ALL):{
            
            //write(path,data,STORAGE_DEVICE::MICROSD);
            //write(path,data,STORAGE_DEVICE::FLASH);

            break;
        }
        case(STORAGE_DEVICE::MICROSD):{
            
            file = microsd.open(path.c_str(), (O_WRITE | O_CREAT | O_AT_END));//
            if (file){
                //check if file is okay
                file.print(data.c_str());
                file.close();//close the file
            }
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

