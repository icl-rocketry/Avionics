#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#include <string>
#include "storageController.h"
#include "logController.h"

#include <ArduinoJson.h>



class ConfigController{
    public:
        ConfigController(StorageController& storagecontroller,LogController& logcontroller);
        void load();
        
        DynamicJsonDocument configDoc;
        bool _error;
    private:
        StorageController& _storagecontroller; //pointer to storage controller
        LogController& _logcontroller;//pointer to log controller
        const std::string configuration_file_path = "/Configuration/rml.jsonc";

        
};

#endif
