#ifndef STORAGE_H
#define STORAGE_H
//object defining a data storage object

#include <Arduino.h>

//probably can remove one or 2 of thees later

#include "FS.h"
#include "SD.h"
#include "SPI.h"


class Storage{

public:
    Storage();

    void setup();
    void ls();

    void write();
    void read();

private:
     
    fs::SDFS _flash;

};





#endif