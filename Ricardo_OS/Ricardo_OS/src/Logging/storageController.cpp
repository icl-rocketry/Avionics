#include "storageController.h"
#include "stateMachine.h"
#include "ricardo_pins.h"
#include "utils.h"

#include <Arduino.h>
#include <string>
#include <SPI.h>
#include <SdFat.h>
#include <cctype>

#include "flags.h"
#include "spiFlashConfig.h"


StorageController::StorageController(stateMachine* sm):
_sm(sm),
microsd(&(sm->vspi)),
flashTransport(FlashCs,&(sm->vspi)),
flash(&flashTransport)
{};

bool StorageController::setup(){
 

    if(!microsd.begin(SdCs,SD_SCK_MHZ(10))){
        _sm->systemstatus.new_message(system_flag::ERROR_SD,"Error intializing SD card");
        return false;
    }
    _sm->logcontroller.log("SD Initalized");

    SPIFlash_Device_t flash_config = W25Q128JV_SM; //pass in spi flash config
    
    if(!flash.begin(&flash_config)){
        _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error intializing onboard flash");
        return false;
    }
    _sm->logcontroller.log("Flash Initalized");
    
    if(!flash_fatfs.begin(&flash)){
        _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error intializing onboard flashfs");
        return false;
    }
    _sm->logcontroller.log("Flash FS Initalized");
  
    return true;

};

std::string StorageController::updateDirectoryName(std::string input_directory,STORAGE_DEVICE device){
    // Looks for the highest numbered log folder and increments by one
    std::vector<directory_element_t> fileNames = std::vector<directory_element_t>();

    if (!ls(input_directory, fileNames, device)) {
        // TODO: there's an error, crash ourseleves
    }
    
// go away u a big poo
// rude
// <3
    std::string maxDirName = "0";
    int maxDigitIndex = maxDirName.length()-1, digitIndex = 0; // maxDigitIndex is 4 because 0 is the 4th place in logs0
    int maxDirNum = 0, dirNum = 0;


    for (directory_element_t elem : fileNames) {
        std::string fName = elem.name;

        digitIndex = getFileNameIndex(fName);
        dirNum = utils::intify(fName.substr(digitIndex, fName.length() - digitIndex));

        //Serial.println(dirNum);
        if (dirNum > maxDirNum) {

            maxDirNum = dirNum;
            maxDirName = fName;
            maxDigitIndex = digitIndex;
        }
    }

    return input_directory + "/" + maxDirName.substr(0, maxDigitIndex) + utils::tostring(maxDirNum + 1);
}

const int StorageController::getFileNameIndex(const std::string fileName) {
    int numberStartIdx; // Start index of the character at which the digit index starts
    for (numberStartIdx = fileName.length() - 1; numberStartIdx >=0; --numberStartIdx) {
        if (!std::isdigit(fileName.at(numberStartIdx))) {
            --numberStartIdx;
            break;
        }
    }
    //return utils::intify(fileName.substr(numberStartIdx, fileName.length() - numberStartIdx));
    return numberStartIdx+1; 
}

void StorageController::mkdir(std::string path,STORAGE_DEVICE device){
    switch(device){
        case STORAGE_DEVICE::MICROSD:{
            if(!microsd.mkdir(path.c_str())){
                microsd.mkdir(path.c_str());
            }else{
                _sm->logcontroller.log(path + " directory already exists");
            }
            break;
        }
        case STORAGE_DEVICE::FLASH:{
            if(!flash_fatfs.mkdir(path.c_str())){
                flash_fatfs.mkdir(path.c_str());
            }else{
                _sm->logcontroller.log(path + " directory already exists");
            }
            break;
        }
        default:{
            break;
        }
    }
}

bool StorageController::ls(std::string path,std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device){
    File _file;
    switch(device){
        case STORAGE_DEVICE::MICROSD:{
            microsd.chvol();//change vol to microsd
            _file = microsd.open(path.c_str()); // open the path supplied
            break;
        }
        case STORAGE_DEVICE::FLASH:{
            flash_fatfs.chvol();//change vol to flash
            _file = flash_fatfs.open(path.c_str()); // open the path supplied
            break;
        }
        default:{
            return false; // return nothing 
        }
    }
    
    if((!_file) || (!_file.isDirectory())){
        //path invalid so return
        
        return false;
    }
    File child = _file.openNextFile();//open next file in directory

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
        child = _file.openNextFile(); // open next file

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
   File file;

   switch(device){
        case(STORAGE_DEVICE::ALL):{
            
            //write(path,data,STORAGE_DEVICE::MICROSD);
            //write(path,data,STORAGE_DEVICE::FLASH);

            break;
        }
        case(STORAGE_DEVICE::MICROSD):{
            microsd.chvol();
            file = microsd.open(path.c_str(), (O_WRITE | O_CREAT | O_AT_END));//
            if (file){
                //check if file is okay
                file.print(data.c_str());
                file.close();//close the file
            }
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            flash_fatfs.chvol();
            file = flash_fatfs.open(path.c_str(), (O_WRITE | O_CREAT | O_AT_END));//
            if (file){
                //check if file is okay
                file.print(data.c_str());
                file.close();//close the file
            }

            break;
        }
        default:{
            //do nothing
            break;
        }
    }
}

File StorageController::read(std::string path,STORAGE_DEVICE device){
    File ret;
    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            microsd.chvol();
            ret = microsd.open(path.c_str());
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            flash_fatfs.chvol();
            ret = flash_fatfs.open(path.c_str());
            break;
        }
        default:{
            //do nothing
            break;
        }
    }
    return ret;
}

bool StorageController::erase(STORAGE_DEVICE device){
    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            if(!microsd.wipe()){
                _sm->systemstatus.new_message(system_flag::ERROR_SD,"Error wiping SD card");
                return false;
            }
            _sm->logcontroller.log("SD Wiped");
            return true;
        }
        case(STORAGE_DEVICE::FLASH):{
            if(!flash_fatfs.wipe()){
                _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error wiping onboard flash");
                return false;
            }   
            _sm->logcontroller.log("Flash Wiped");
            return true;       
        }
        default:{
            //no option supplied so dont do anything
            return false;
        }
    }
}
