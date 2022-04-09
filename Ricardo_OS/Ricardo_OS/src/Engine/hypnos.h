#pragma once

#include "engine.h"

#include <ArduinoJson.h>
#include <memory>

#include "../RocketComponents/rocketactuator.h"

struct HypnosState:public EngineState{

};


class Hypnos:public Engine{
    public:
        Hypnos(uint8_t id,JsonObjectConst engineConfig,addNetworkCallbackF_t addNetworkCallbackF,RnpNetworkManager& networkmanager,uint8_t handlerServiceID,LogController& logcontroller);

        void updateState() override{};

        uint8_t flightCheck() override{};

        void update() override {}; //empty we dont need an update as this a simple on and off

        const EngineState* getState()override {return &_state;};

        void execute(int32_t func) override;

        /**
         * @brief Control the engine throttle by varying valve displacement
         * 
         * @param u single value
         */
        void control(std::vector<float> u) override{};


    private:
        HypnosState _state;

        void ignite() override{};
        void shutdown() override{}; 
        void vent(); // vent main ox tank
        

        static constexpr uint16_t _networkRetryInterval = 5000;

};