#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#include <string>
#include "storageController.h"
#include "logController.h"

#include <ArduinoJson.h>



class ConfigController{
    public:
        ConfigController(StorageController* storagecontroller,LogController* logcontroller);
        
        /**
         * @brief Load configuration and store in configDoc variable 
         * 
         */
        void load();
        
        /**
         * @brief Check if config was read correctly
         * 
         * @return true 
         * @return false 
         */
        operator bool(){return _configOK;};

        JsonObjectConst get(){return configDoc.as<JsonObject>();};
        
    private:
        DynamicJsonDocument configDoc;

        StorageController* _storagecontroller; //pointer to storage controller
        LogController* _logcontroller;//pointer to log controller
        const std::string configuration_file_path = "/Configuration/rml.jsonc";
        bool _configOK;
        
        
};

#endif
