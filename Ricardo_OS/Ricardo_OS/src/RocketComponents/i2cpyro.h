#pragma once
/**
 * @file i2cpyro.h 
 * @author Kiran de Silva
 * @brief Class implementing a i2c pyro device based on the pca9534. Channels 0-3 refer to both nuke and continuity pins
 * @version 0.1
 * @date 2022-05-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "rocketactuator.h"
#include "rocketcomponenttype.h"

#include "Storage/logController.h"

#include <Wire.h>

class I2CPyro: public RocketActuator{
    public:

        /**
         * @brief Construct a new I2CPyro.
         * 
         * @param id 
         * @param logcontroller 
         * @param address 
         * @param channel pyro channels 0-3
         * @param wire 
         */
        I2CPyro(uint8_t id,LogController& logcontroller,uint8_t address,uint8_t channel,bool continuityPolarity,TwoWire &wire);

         /**
         * @brief Fires the Pyro
         * 
         * @param param - fire time in ms
         */
        void execute(int32_t param) override;

        /**
         * @brief Read the status of contPin
         * 
         */
        void updateState() override;

        /**
         * @brief Returns the last received state from the actuator
         * 
         * @return const ComponentState* 
         */
        const RocketComponentState* getState() override {return &_state;};


    private:
        const uint8_t _address;
        const uint8_t _channel;
        bool pinsValid;
        const uint8_t _nukePin;
        const uint8_t _continuityPin;
        const bool _continuityPolarity;
        

        TwoWire& _wire;

        RocketActuatorState _state;

        static constexpr uint8_t INPUT_PORT = 0x00;
        static constexpr uint8_t OUTPUT_PORT = 0x01;
        static constexpr uint8_t POLARITY = 0x02;
        static constexpr uint8_t CONFIG = 0x03;

        /**
         * @brief Helper function to get nuke pin from channel. As always, the pinout is 
         * reverse to the channel numbering so yeah.
         * 
         * @param channel 
         * @return uint8_t 
         */
        uint8_t get_nuke_pin(uint8_t channel);
        /**
         * @brief Helper function to get continuity pin from channel.
         * 
         * @param channel 
         * @return uint8_t 
         */
        uint8_t get_continuity_pin(uint8_t channel);

        /**
         * @brief Get the value in register
         * 
         * @param reg 
         * @return uint8_t 
         */
        uint8_t get_register(uint8_t reg);

        

};