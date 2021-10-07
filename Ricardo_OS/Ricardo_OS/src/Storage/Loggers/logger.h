#pragma once

//abstract interface to logger type clases

#include <SdFat.h>
#include "../storageController.h"

class LogFile{
    public:
        LogFile(STORAGE_DEVICE dev,StorageController &sc);

        bool write(const void* buf, size_t len);
        void open(const std::string filepath);
        void close();
        void flush();
        STORAGE_DEVICE getDevice(){return device;};

        
        

    private:
        StorageController &_sc;

        File file;
        STORAGE_DEVICE device;
        int status;
        std::string curr_filepath;
};

class Logger{
    public:
        Logger(StorageController *sc,std::string filename,STORAGE_DEVICE mainStorage = STORAGE_DEVICE::MICROSD,STORAGE_DEVICE backupStorage = STORAGE_DEVICE::FLASH);
        virtual ~Logger();

        virtual void enable(); // this method will open the log file
        virtual void disable(); // this method will close the log file

        virtual void flush() = 0; // purely virtual
        

        bool status(){return _status;};

        void setDirectory(std::string parent_directory);
        

    protected:
        StorageController* _sc;//pointer to storage controller

        LogFile main_logfile;
        LogFile backup_logfile;

        std::string _filename;
        std::string _filepath;

        bool _status;
        bool _backupEnabled;

      
        
};
