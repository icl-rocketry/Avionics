#pragma once
//persistent store creates an interface to nvs storage using onboard esp32 flash
// this allows key value pairs to be stored persistently on esp32
#include "Storage/logController.h"
#include "nvs.h"
#include <string>

class PersistentStore{
    public:
        PersistentStore(std::string name,LogController& logcontroller);

        esp_err_t setBytes(std::string key,uint8_t* data,size_t size);
        esp_err_t getBytes(std::string key,uint8_t* data,size_t size);

        template<typename T>
        esp_err_t set(std::string key,T value){
          return setBytes(key,(uint8_t*)value,sizeof(T));
        };

        template<typename T>
        esp_err_t get(std::string key,T &variable){
            return getBytes(key,(uint8_t*)variable,sizeof(T));
        };

        ~PersistentStore();


    private:
        LogController& _logcontroller; // reference to log controller

        nvs_handle_t _nvs_handle;



};

//template specializations - implementation in source
template<>
esp_err_t PersistentStore::set<int32_t>(std::string key,int32_t value);
template<>
esp_err_t PersistentStore::get<int32_t>(std::string key,int32_t &variable);
