#include "logger.h"
#include "../storageController.h"
#include <SdFat.h>

Logger::Logger(StorageController *sc,uint16_t dt,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
_sc(sc),
_mainStorage(mainStorage),
_backupStorage(backupStorage),
_writeDelta(dt),
_filename(filename),
_status(false),
{};

void Logger::enable(){
    if (_filepath.size() != 0){ // ensure a filepath has been provided otherwise do not allow logger to be enabled
        if (!_status){//check if logger is already running
            main_logfile = _sc->open(_filepath,_mainStorage,(O_WRITE | O_CREAT | O_AT_END));
            backup_logfile = _sc->open(_filepath,_backupStorage,(O_WRITE | O_CREAT | O_AT_END));
            _status = true; //set status to enabled
        }
    }else{
        _status = false;
    }
};

void Logger::disable(){
    main_logfile.close();
    backup_logfile.close();
    _status = false;
};

void Logger::setDirectory(std::string parent_directory){
    _filepath = parent_directory + _filename;
};


