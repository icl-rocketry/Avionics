#pragma once

//abstract interface to logger type clases

#include <SdFat.h>
#include "../storageController.h"

class Logger{
    public:
        Logger(StorageController *sc,uint16_t dt,std::string filename,STORAGE_DEVICE mainStorage = STORAGE_DEVICE::MICROSD,STORAGE_DEVICE backupStorage = STORAGE_DEVICE::FLASH);
        virtual ~Logger();

        virtual void enable(); // this method will open the log file
        virtual void disable(); // this method will close the log file

        virtual void writeLog() = 0; // purely virtual
        

        bool status(){return _status;};
        void changeFrequency(uint16_t dt){_writeDelta = dt;};
        void setDirectory(std::string parent_directory);
        

    protected:
        StorageController* _sc;//pointer to storage controller

        STORAGE_DEVICE _mainStorage;
        STORAGE_DEVICE _backupStorage;

        File main_logfile;
        File backup_logfile;

       

        uint16_t _writeDelta;
        uint64_t _prevWriteTime;

        std::string _filename;
        std::string _filepath;

        bool _status;
        bool _backupEnabled;
};
