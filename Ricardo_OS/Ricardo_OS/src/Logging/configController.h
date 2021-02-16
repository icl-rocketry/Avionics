#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#include <string>
#include "storageController.h"


class ConfigController{
    public:
        ConfigController(StorageController *storagecontroller);
    private:
        StorageController *_storagecontroller; //pointer to storage controller
};

#endif