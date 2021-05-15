#include "storageController.h"
#include "stateMachine.h"
#include "ricardo_pins.h"
#include "utils.h"

#include <Arduino.h>
#include <string>
//#include <SPI.h>
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

void StorageController::setup(){
 
    if(microsd.begin(SdCs,SD_SCK_MHZ(10))){
        generateDirectoryStructure(STORAGE_DEVICE::MICROSD);
        _sm->logcontroller.log("SD Initalized");   
    }else{
        _sm->systemstatus.new_message(system_flag::ERROR_SD,"Error intializing SD card");
    }
    

    const SPIFlash_Device_t flash_config = W25Q128JV_SM; //pass in spi flash config
    
    if(flash.begin(&flash_config)){

        _sm->logcontroller.log("Flash Initalized");

        if(flash_fatfs.begin(&flash)){
            generateDirectoryStructure(STORAGE_DEVICE::FLASH);
            _sm->logcontroller.log("Flash FS Initalized");
        
        }else{
            _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error intializing onboard flashfs");
            
        }

    }else{
        _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error intializing onboard flash");
        
    }
    
};

std::string StorageController::getUniqueDirectory(std::string input_directory,STORAGE_DEVICE device){
    // Looks for the highest numbered log folder and increments by one
    std::vector<directory_element_t> fileNames = std::vector<directory_element_t>();

    mkdir(input_directory,device); // ensure directory exists

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

int StorageController::getFileNameIndex(const std::string fileName) {
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

bool StorageController::mkdir(std::string path,STORAGE_DEVICE device){
    bool alreadyPresent = false;
    switch(device){
        case STORAGE_DEVICE::MICROSD:{
            if(!microsd.exists(path.c_str())){
                microsd.mkdir(path.c_str());
            }else{
                _sm->logcontroller.log(path + " directory already exists");
                alreadyPresent = true;
            }
            break;
        }
        case STORAGE_DEVICE::FLASH:{
            if(!flash_fatfs.exists(path.c_str())){
                flash_fatfs.mkdir(path.c_str());
            }else{
                _sm->logcontroller.log(path + " directory already exists");
                alreadyPresent = true;
            }
            break;
        }
        default:{
            break;
        }

    }
    return alreadyPresent;
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
    //all log files in the log controller must be closed from this point for ls to function correctly
    // i dont like the coupling this creates but its currently the best way. will think of a better way later
    _sm->logcontroller.stopLogging(LOG_TYPE::ALL);

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
    // re open log files in log controller here
    _sm->logcontroller.startLogging(LOG_TYPE::ALL);

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


File StorageController::open(std::string &path, STORAGE_DEVICE device,oflag_t mode){
    File ret;
    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            microsd.chvol();
            ret = microsd.open(path.c_str(),mode);
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            flash_fatfs.chvol();
            ret = flash_fatfs.open(path.c_str(),mode);
            break;
        }
        default:{
            //do nothing
            break;
        }
    }
    return ret;
}

bool StorageController::erase(STORAGE_DEVICE device){ // need to make sure this doesnt wipe out configuration files lol
//maybe call the configcontroller to rewrite the config file if we store the configuration in ram
    // cross coupling here again because we need to turn off logging so we can update the log file paths
    bool error = false;

    _sm->logcontroller.stopLogging(LOG_TYPE::ALL);

    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            if(!microsd.wipe()){
                _sm->systemstatus.new_message(system_flag::ERROR_SD,"Error wiping SD card");
                error = true;
                break;
            }
            
            _sm->logcontroller.log("SD Wiped");
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            if(!flash_fatfs.wipe()){
                _sm->systemstatus.new_message(system_flag::ERROR_FLASH,"Error wiping onboard flash");
                error = true;
                break;
            }   

            _sm->logcontroller.log("Flash Wiped");
            break;       
        }
        default:{
            //no option supplied so dont do anything
            error = true;
            break;
        }
    }
    //make sure directory structure is present on wiped device
    generateDirectoryStructure(device);
    _sm->logcontroller.generateLogDirectories(device); //ensure logging directories exist
    //reopen log files
    _sm->logcontroller.startLogging(LOG_TYPE::ALL);    

    return error;
    
}

void StorageController::generateDirectoryStructure(STORAGE_DEVICE device){
    //ensure the expected directory structure is present
    /*

    /
    /Logs
    /Configuration
    */
   mkdir("/Logs",device);
   mkdir("/Configuration",device);
}