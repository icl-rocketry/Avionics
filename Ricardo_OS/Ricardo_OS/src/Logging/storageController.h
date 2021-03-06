#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H


#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <string>
#include <vector>



class stateMachine; //forward declaration to prevent ciruclar dependancy

enum class STORAGE_DEVICE:uint8_t{
    ALL, //all writes to all avalibale memory devices to backup
    MICROSD,
    FLASH 


};

enum class FILE_TYPE:uint8_t{
    FILE,
    DIRECTORY
};

struct directory_element_t{
    std::string name;
    uint32_t size; //?
    FILE_TYPE type;
};


class StorageController{
    public:
        StorageController(stateMachine *sm);
        bool setup(); //setup storage devices

        void write(std::string &path,std::string &data,STORAGE_DEVICE device);//missing arguemtn for data -we need to decide 
        void read(std::string path,STORAGE_DEVICE device); // need to figure out return type

        std::string updateDirectoryName(std::string input_directory,STORAGE_DEVICE device); //checks and updates directory name so that it is unique

        bool ls(std::string path,std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device);
        bool ls(std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device);
        void printDirectory(std::string path,STORAGE_DEVICE device);
        bool format(STORAGE_DEVICE device);//format device

    private:
        stateMachine *_sm;//pointer to state machine

        SdFat microsd; // microsd card storage object
        File file; // file object
        
        Adafruit_FlashTransport_SPI flashTransport;
        Adafruit_SPIFlash flash;// flash storage object

        FatFileSystem flash_fatfs;//flash filesystem
        
        

        
        
};


#endif