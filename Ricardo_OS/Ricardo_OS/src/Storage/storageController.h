#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H


#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <string>
#include <vector>
#include <SdFat.h>



class stateMachine; //forward declaration to prevent ciruclar dependancy

enum class STORAGE_DEVICE:uint8_t{
    MICROSD,
    FLASH,
    NONE 
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
        void setup(); //setup storage devices

        //void write(std::string &path,std::string &data,STORAGE_DEVICE device);//missing arguemtn for data -we need to decide 
        //File read(std::string path,STORAGE_DEVICE device); 

        File open(std::string path,STORAGE_DEVICE device,oflag_t mode);

        std::string getUniqueDirectory(std::string input_directory,STORAGE_DEVICE device); //checks and updates directory name so that it is unique
        
        bool mkdir(std::string path, STORAGE_DEVICE device);
        bool ls(std::string path,std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device);
        bool ls(std::vector<directory_element_t> &directory_structure,STORAGE_DEVICE device);
        void printDirectory(std::string path,STORAGE_DEVICE device);
        bool erase(STORAGE_DEVICE device);//format device

        void generateDirectoryStructure(STORAGE_DEVICE device);

        


    private:
        stateMachine *_sm;//pointer to state machine

        SdFat microsd; // microsd card storage object
        //File file; // file object
        
        Adafruit_FlashTransport_SPI flashTransport;
        Adafruit_SPIFlash flash;// flash storage object

        FatFileSystem flash_fatfs;//flash filesystem
        
        int getFileNameIndex(const std::string fileName); // Extracts the index of the file from file name (e.g. returns 4 when input is logs_flight_4)
        bool ls(std::string path,std::vector<directory_element_t> &directory_structure,FatFileSystem* fs);
       
        bool rmParent(std::string path, FatFileSystem* fs); // remove all files under directory -> deletes path
        
        
};


#endif