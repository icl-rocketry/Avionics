#include "configController.h"
#include "storageController.h"
//#include <ArduinoJson.h>

ConfigController::ConfigController(StorageController *storagecontroller):
_storagecontroller(storagecontroller)
{};

void ConfigController::setup(){
// check if config file exissts on sd - if not create one?

}

