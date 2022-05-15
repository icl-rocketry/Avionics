#pragma once

#include <ArduinoJson.h>
#include <exception>


namespace JsonConfigHelper{
    template<typename T>
    static void setIfContains(const JsonVariantConst config, const std::string& key, T& var, bool throwIfDoesntContain = true) {
        if (config.containsKey(key)) {
            var = config[key].as<T>();
        } else {
            if (throwIfDoesntContain)
                throw std::runtime_error("Key : " + key + " missing from config\n");
        }
    }

    template<typename T>
    static T getIfContains(const JsonVariantConst config, const std::string& key, bool throwIfDoesntContain=true) {
        if (config.containsKey(key)) {
           return config[key].as<T>();
        } else {
            if (throwIfDoesntContain)
                throw std::runtime_error("Key : " + key + " missing from config\n");
        }
        return {};
    }

    static inline bool checkConfigId(size_t id,JsonObjectConst config, bool throwIfDoesntContain = true){
        auto jsonID = getIfContains<size_t>(config,"id",throwIfDoesntContain);
        if (jsonID != id){
            if (throwIfDoesntContain){
                throw std::runtime_error("ID mismatch, ID:" + std::to_string(id) + " json ID:" + std::to_string(jsonID));
            }
            return false;
        }
        return true;
    }


};
