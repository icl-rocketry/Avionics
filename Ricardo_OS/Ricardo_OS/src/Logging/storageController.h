#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H


#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include "stateMachine.h"
#include <string>
#include "flash_devices.h"//flash_device configuration defintion

enum class STORAGE_DEVICE:uint8_t{
    ALL = 0, //all writes to all avalibale memory devices to backup
    MICROSD = 1,
    FLASH = 2


};


class StorageController{
    public:
        StorageController(stateMachine *sm);
        bool setup(); //setup storage devices

        void write(std::string path,std::string data,STORAGE_DEVICE device);//missing arguemtn for data -we need to decide 
        void read(std::string path,STORAGE_DEVICE device); // need to figure out return type
    
        bool format(STORAGE_DEVICE device);//format device

    private:
        stateMachine *_sm;//pointer to state machine

        MicroSD microsd; // microsd card storage object
        


        Adafruit_FlashTransport_SPI spi_flash;
        Adafruit_SPIFlash flash;// flash storage object
        SPIFlash_Device_t flash_config; //specific flash config
        FatFileSystem flash_fatfs;//flash filesystem

        
        
};


#endif