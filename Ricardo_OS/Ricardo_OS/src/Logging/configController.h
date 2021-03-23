#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#include <string>
#include "storageController.h"

#include <ArduinoJson.h>



class ConfigController{
    public:
        ConfigController(StorageController *storagecontroller);
        void setup();
        
        
    private:
        StorageController *_storagecontroller; //pointer to storage controller
        const std::string configuration_file_path = "/Config.json";
};

#endif
