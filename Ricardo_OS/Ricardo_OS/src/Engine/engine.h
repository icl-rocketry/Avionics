#pragma once
/**
 * @file engine.h
 * @author Kiran de Silva
 * @brief Engine Base class
 * @version 0.1
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <memory>
#include <unordered_map>
#include <vector>
#include <utility>

#include <rnp_networkmanager.h>
#include <ArduinoJson.h>

#include "Controller/controllable.h"

#include <rnp_networkcallbackmap.h>
#include "RocketComponents/rocketcomponent.h"

#include "Storage/logController.h"

enum class ENGINE_RUN_STATE:uint8_t{
    SHUTDOWN,
    RUNNING,
    ERROR
};

enum class ENGINE_CONNECTION_STATE:uint8_t{
    CONNECTED,
    ERROR
};

struct EngineState{
    uint8_t runState;
    uint8_t connectionState;
    uint32_t ignitionTime;
    uint32_t shutdownTime;
};
using addNetworkCallbackF_t = std::function<void(uint8_t,uint8_t,std::function<void(std::unique_ptr<RnpPacketSerialized>)>,bool)>;
class Engine : public Controllable {
    public:
        /**
         * @brief Construct a new Engine object, setup the engine, as the engine also could contain networked compoentns, a reference to he addNetworkCallback function is passed by reference so we can add
         * the appropriate callback allowing incomming packets to be routed to the correct component.
         * 
         * @param id 
         * @param engineConfig 
         * @param addNetworkCallbackF 
         * @param networkmanager 
         * @param handlerServiceID 
         * @param logcontroller 
         */
        Engine(uint8_t id,[[maybe_unused]] JsonObjectConst engineConfig,[[maybe_unused]] addNetworkCallbackF_t addNetworkCallbackF,RnpNetworkManager& networkmanager,uint8_t handlerServiceID,LogController& logcontroller):
        _id(id),
        _networkmanager(networkmanager),
        _handlerServiceID(handlerServiceID),
        _logcontroller(logcontroller)
        {};

        /**
         * @brief Requests a a state update for all components in engine
         * 
         */
        virtual void updateState() = 0;
        /**
         * @brief Performs Flight check for all components in the engine
         * 
         * @return uint8_t is engine in error
         */
        uint8_t flightCheckEngine(){return flightCheck() ? 1 : 0;};
        /**
         * @brief Performs flight check on engine
         * 
         * @return uint8_t number of engine components in error
         */
        virtual uint8_t flightCheck() = 0;
        
        virtual void update() = 0;

        /**
         * @brief Engine functor 
         *          param = 0 is shutdown
         *          param = 1 is ignition
         *          param = n is defined by the derived implementation i.e in hypnos p3 = vent main ox
         * 
         * @param func 
         */
        virtual void execute(int32_t func);

        virtual const EngineState* getState() = 0;

        virtual ~Engine();

        uint8_t getID(){return _id;};

    protected:
        const uint8_t _id;
        
        RnpNetworkManager& _networkmanager;

        const uint8_t _handlerServiceID;

        LogController& _logcontroller;



       
        

        // all engines will have these methods
        virtual void ignite(); 
        virtual void shutdown();
};