#include "logger.h"
#include "../storageController.h"
#include <SdFat.h>


LogFile::LogFile(STORAGE_DEVICE dev,StorageController &sc):
_sc(sc),
device(dev)
{}

bool LogFile::write(const void* buf, size_t len) 
{
    if (device != STORAGE_DEVICE::NONE){//dont attempty to write if we are a none device
        int current_status  = file.write(buf,len);
        if (current_status == -1){
            //error try reopen the file at the most recently used filepath and report error
            if (!curr_filepath.empty()){
                open(curr_filepath);
            }
            if (status != -1) _sc.reportStatus(device,StorageController::DEVICE_STATE::ERR_WRITE);
            status = current_status;
            return false;
        }

        if (status == -1){
            _sc.reportStatus(device,StorageController::DEVICE_STATE::OK);
            status = current_status;
        }
        
        return true;
    }
    return true;
}

void LogFile::open(const std::string filepath) 
{
    curr_filepath = filepath;
    file = _sc.open(filepath,device,(O_WRITE | O_CREAT | O_AT_END));
}

void LogFile::close() 
{
    file.close();
};

void LogFile::flush()
{
    file.flush();
};


Logger::Logger(StorageController *sc,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
_sc(sc),
main_logfile(mainStorage,*sc),
backup_logfile(backupStorage,*sc),
_filename(filename),
_status(false)
{};

Logger::~Logger(){};

void Logger::enable(){
    if (_filepath.size() != 0){ // ensure a filepath has been provided otherwise do not allow logger to be enabled
        if (!_status){//check if logger is already running
            main_logfile.open(_filepath);
            backup_logfile.open(_filepath);
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
}





