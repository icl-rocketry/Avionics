#pragma once

#include "configurabledynamichandler.h"


#include <unordered_map>
#include <functional>


#include <rnp_networkmanager.h>

#include <rnp_networkcallbackmap.h>

#include "stubs.h"


template<class Object,class Derived>
class NetworkedDynamicHandler : public ConfigurableDynamicHandler<Object,Derived>{
    public:
        NetworkedDynamicHandler(RnpNetworkManager& networkmanager,uint8_t serviceID,LogController& logcontroller):
        ConfigurableDynamicHandler<Object,Derived>(logcontroller),
        _networkmanager(networkmanager),_serviceID(serviceID)
        {};


         /**
         * @brief Default Callback Implementation can be overriden if needed
         * 
         * @param packetptr 
         */
        void networkCallback(packetptr_t packetptr){
            static_cast<Derived*>(this)->networkCallback_impl(std::move(packetptr));
        };

        /**
         * @brief Returns network callback bound to instance
         * 
         * @return std::function<void(packetptr_t)> 
         */
        std::function<void(packetptr_t)> getThisNetworkCallback(){return [this](packetptr_t packetptr){networkCallback(std::move(packetptr));};};

    protected:
        RnpNetworkManager& _networkmanager;
        const uint8_t _serviceID;


        /**
         * @brief Adds provided network callback to map. Care needs to be taken to ensure 
         * object referenced in the callback exists otherwise UB.
         * 
         * @param source 
         * @param source_service 
         * @param callback 
         * @param OnError 
         */

        void addNetworkCallback(uint8_t source,uint8_t source_service,std::function<void(std::unique_ptr<RnpPacketSerialized>)> callback,bool throwOnError = true){
            if (_networkCallbackMap.count({source,source_service})){
                if (throwOnError){
                    throw std::runtime_error("Callback already exists in map!, source:" + std::to_string(source) + " source_service:" + std::to_string(source_service));
                }
                return;
            }
            _networkCallbackMap.insert({{source,source_service},callback});
        };
        
    
        /**
         * @brief Returns reference to the callback function pointer
         * 
         * @param source 
         * @param source_service 
         * @return auto& 
         */
        auto& getNetworkCallback(uint8_t source,uint8_t source_service){  
           return _networkCallbackMap.at({source, source_service});
        }

        void networkCallback_impl(packetptr_t packetptr){
            try {
                getNetworkCallback(packetptr->header.source, packetptr->header.source_service)(std::move(packetptr));
            } catch (const std::out_of_range& e) {
                #ifdef _RICDEBUG 
                throw std::runtime_error("callback not found");
                #endif
                this->_logcontroller.log("callback not found");
                return;
            }
        };
    
    private:

        /**
         * @brief deletes a callback. This really shouldnt be called but is provided for completeness
         * 
         * @param source 
         * @param source_service 
         */
        void removeNetworkCallback(uint8_t source,uint8_t source_service){
            _networkCallbackMap.erase({source,source_service});
        };

        /**
         * @brief Hash map to map sourceservice and node address to the correct network callback
         * 
         */
        NetworkCallbackMap_t _networkCallbackMap{1,NetworkCallbackMap::pair_hash_func};

};