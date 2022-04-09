#pragma once

#include "dynamichandlerbase.h"

#include <vector>
#include <string>
#include <memory>

#include <ArduinoJson.h>


#include "stubs.h"

namespace ConfigurableDynamicHandlerHelpers{
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
    static T getIfContains(const JsonVariantConst config, const std::string& key, bool throwIfDoesntContain = true) {
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


template<class Object,class Derived>
class ConfigurableDynamicHandler : public DynamicHandlerBase<Object,Derived> {
    public:
        ConfigurableDynamicHandler(LogController& logcontroller):
        DynamicHandlerBase<Object,Derived>(logcontroller)
        {};

        /**
         * @brief Can be overriden by derived in special cases but should suffice for most 
         * handler implementations. setupIndividual_impl called by use of crtp to remove virtual
         * dispatch overhead.
         * 
         * @param handlerconfig json array containing the config of the obejcts
         */
        void setup(JsonArrayConst handlerconfig)
        {
            static_cast<Derived*>(this)->setup_impl(handlerconfig);
        };

    protected:

        void setup_impl(JsonArrayConst handlerconfig){
            for (JsonObjectConst config: handlerconfig){
                size_t id = this->size();
                ConfigurableDynamicHandlerHelpers::checkConfigId(id,config);
                static_cast<Derived*>(this)->setupIndividual_impl(id,config);
            }
        }

};