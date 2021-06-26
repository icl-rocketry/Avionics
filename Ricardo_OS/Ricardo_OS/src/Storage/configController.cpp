#include "configController.h"
#include "storageController.h"
#include "logController.h"
#include <ArduinoJson.h>
#include <SdFat.h>
#include <string.h>



ConfigController::ConfigController(StorageController& storagecontroller,LogController& logcontroller):
configDoc(16384),
_error(false),
_storagecontroller(storagecontroller),
_logcontroller(logcontroller)
{};

void ConfigController::load(){
    File _file; // file object
    _file = _storagecontroller.open(configuration_file_path.c_str(),STORAGE_DEVICE::MICROSD,O_RDONLY); //try to open file at directory
    if(!_file){
        _error = true;
        _logcontroller.log("Error opening config file!");
        return;
    }
    DeserializationError jsonError = deserializeJson(configDoc,_file);
    if (jsonError){
        _error = true;
        _logcontroller.log("Error deserializing JSON! - " + std::string(jsonError.c_str()));
        return;
    }
    _error = false;
}

