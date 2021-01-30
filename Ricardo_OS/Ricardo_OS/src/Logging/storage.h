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

    virtual void setup();

    //file system funcitons
    void ls(fs::FS &fs, const char *dirname, uint8_t levels);
    void cd();
    

    void write();
    void read();

private:

};





#endif