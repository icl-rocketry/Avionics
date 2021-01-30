#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "microSD.h"
#include "flash.h"
#include <string>

enum class STORAGE_DEVICE:uint8_t{
    ALL = 0, //all writes to all avalibale memory devices to backup
    MICROSD = 1,
    FLASH = 2


};


class StorageController{
    public:
        void write(std::string path,std::string data,STORAGE_DEVICE device);//missing arguemtn for data -we need to decide 
        void read(std::string path,STORAGE_DEVICE device); // need to figure out return type
    private:
        MicroSD microsd; // microsd card storage object
        Flash flash; // flash storage object
        
};


#endif