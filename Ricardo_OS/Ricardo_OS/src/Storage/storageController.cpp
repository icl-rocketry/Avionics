#include "storageController.h"
#include "stateMachine.h"
#include "ricardo_pins.h"


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
        _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_SD,"Error intializing SD card");
    }
    

    const SPIFlash_Device_t flash_config = W25Q128JV_SM; //pass in spi flash config
    
    if(flash.begin(&flash_config)){

        _sm->logcontroller.log("Flash Initalized");

        if(flash_fatfs.begin(&flash)){
            generateDirectoryStructure(STORAGE_DEVICE::FLASH);
            _sm->logcontroller.log("Flash FS Initalized");
        
        }else{
            _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_FLASH,"Error intializing onboard flashfs");
            
        }

    }else{
        _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_FLASH,"Error intializing onboard flash");
        
    }
    
};

std::string StorageController::getUniqueDirectory(std::string input_directory,STORAGE_DEVICE device){
    // Looks for the highest numbered log folder and increments by one
    std::vector<directory_element_t> fileNames;

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
        dirNum = std::stoi(fName.substr(digitIndex, fName.length() - digitIndex));

        //Serial.println(dirNum);
        if (dirNum > maxDirNum) {

            maxDirNum = dirNum;
            maxDirName = fName;
            maxDigitIndex = digitIndex;
        }
    }

    return input_directory + "/" + maxDirName.substr(0, maxDigitIndex) + std::to_string(maxDirNum + 1);
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
    bool status;
    switch(device){
        case STORAGE_DEVICE::MICROSD:{
            ls(path,directory_structure,&microsd);
            status = true;
            break;
        }
        case STORAGE_DEVICE::FLASH:{
            ls(path,directory_structure,&flash_fatfs);
            status = true;
            break;
        }
        default:{
            status = false;
            break;
        }
    }
    return status;
};

bool StorageController::ls(std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device){
    std::string path = "/";
    return ls(path,directory_structure, device);
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
        
        


File StorageController::open(std::string path, STORAGE_DEVICE device,oflag_t mode){
    File ret;
    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            microsd.chvol();
            ret = microsd.open(path.c_str(),mode); // this opens the file if a storage device is present or not. Errors are not reported
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

bool StorageController::erase(STORAGE_DEVICE device){ 

    bool error = false;

    _sm->logcontroller.stopLogging(LOG_TYPE::ALL);

    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            if(!rmParent("/Logs", &microsd)){
                _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_SD,"Error wiping SD card");
                error = true;
                break;
            }
            
             _sm->logcontroller.log("SD Wiped");
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            if(!rmParent("/Logs", &flash_fatfs)){
                _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_FLASH,"Error wiping onboard flash");
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
    _sm->logcontroller.setup(); // reinitialize log controller
   
    return error;
    
}

bool StorageController::format(STORAGE_DEVICE device){ 
    bool error = false;

    _sm->logcontroller.stopLogging(LOG_TYPE::ALL);

    switch(device){
        case(STORAGE_DEVICE::MICROSD):{
            if(!microsd.wipe()){
                _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_SD,"Error formatting SD card");
                error = true;
                break;
            }
            
             _sm->logcontroller.log("SD Wiped");
            break;
        }
        case(STORAGE_DEVICE::FLASH):{
            if(!flash_fatfs.wipe()){
                _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_FLASH,"Error formatting onboard flash");
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
    _sm->logcontroller.setup(); // reinitialize log controller
  
    return error;
    
}

bool StorageController::ls(std::string path,std::vector<directory_element_t> &directory_structure,FatFileSystem* fs){
    File _file;
    fs->chvol(); // cahnge vol to fs provided
    _file = fs->open(path.c_str()); //open supplied path
    
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
}

bool StorageController::rmParent(std::string path, FatFileSystem* fs) {    

    // ls path
    // for each in path
    //      if isDir
    //          call rmParent
    //      if isFile
    //          nuke it
    // nuke parent dir as it should be empty
    std::vector<directory_element_t> contents;
    ls(path, contents, fs);

    for (directory_element_t element : contents) {
        std::string element_path = path + '/' + element.name;
        if (element.type == FILE_TYPE::DIRECTORY){
            rmParent(element_path, fs);
        } else {
            fs->remove(element_path.c_str());
        }   
    }
    
    fs->rmdir(path.c_str());
    return true;
};
    

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

void StorageController::reportStatus(STORAGE_DEVICE device,DEVICE_STATE state) 
{
    SYSTEM_FLAG flag;
    //if there ever is more devices, we need some mapping between flags and storage device
    switch(device){
        case STORAGE_DEVICE::MICROSD:
        {
            flag = SYSTEM_FLAG::ERROR_SD;
            break;
        }
        case STORAGE_DEVICE::FLASH:
        {
            flag = SYSTEM_FLAG::ERROR_FLASH;
            break;
        }
        default:
            return;   
    }

    std::string message;
    switch(state){
        case DEVICE_STATE::OK:
        {
            _sm->systemstatus.delete_message(flag,"Device is OK!");
            return;
        }
        case DEVICE_STATE::ERR_WRITE:
        {
            message = "Device error writing!";
            break;
        }
        case DEVICE_STATE::ERR_READ:
        {
            message = "Device error reading!";
            break;
        }
        case DEVICE_STATE::ERR_OPEN:
        {
            message = "Device error opening!";
            break;
        }
        default:
            message = "Device unkown error!";
            break;
    }
    _sm->systemstatus.new_message(flag,message);
    
}

