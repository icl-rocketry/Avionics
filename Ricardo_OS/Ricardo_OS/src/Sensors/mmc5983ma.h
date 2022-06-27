#pragma once
/**
 * @file h3lis331dl.h
 * @author Kiran de Silva
 * @brief Sensor class for mmc5983ma
 *
 * 
 * @version 0.1
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>

#include "config.h"

#include "sensorStructs.h"
#include "Storage/logController.h"
#include "Storage/systemstatus.h"

#include "Helpers/axeshelper.h"

#include <Eigen/Core>


struct MagCalibrationParameters{
    float fieldMagnitude;
    float inclination;
    float declination;
    Eigen::Matrix3f A_1;
    Eigen::Vector3f b;
};

class MMC5983MA{
    public:
        /**
         * @brief Construct a new MMC5983MA object using spi bus
         * 
         * @param spi 
         * @param systemstatus 
         * @param logcontroller 
         * @param cs 
         */
        MMC5983MA(SPIClass& spi,uint8_t cs,SystemStatus& systemstatus,LogController& logcontroller);

        /**
         * @brief Construct a new MMC5983MA object using i2c bus 
         * Chip select is required as it selects the communication mode of the
         * MMC5983MA. Pulled High enables i2c.
         * -> for some unkown reason the first pickle rick mmc5983ma doesnt respond over spi hence this class was created
         * 
         * @param wire 
         * @param systemstatus 
         * @param logcontroller 
         * @param scl 
         * @param sda 
         */
        MMC5983MA(TwoWire &wire, uint8_t scl, uint8_t sda,SPIClass &spi, uint8_t cs,SystemStatus &systemstatus, LogController &logcontroller);



        void setup(const std::array<uint8_t,3>& axesOrder,const std::array<bool,3>& axesFlip);

        void update(SensorStructs::MAG_3AXIS_t& data);

        void calibrate(MagCalibrationParameters magCal); // full calibration of mag

    private:

        const bool _useSPI;

        SPIClass* _spi;
        SPISettings _settings;
        const uint8_t _cs;

        TwoWire* _wire;
        const uint8_t _scl;
        const uint8_t _sda;

        SystemStatus& _systemstatus;
        LogController& _logcontroller;
        
        

        AxesHelper<> axeshelper;

        //mag calibration container
        MagCalibrationParameters _magCal;
        void writeMagCal();
        void loadMagCal();

        bool alive();

        void writeRegister(uint8_t reg, uint8_t data);
        void readRegister(uint8_t reg_address, uint8_t *data, uint8_t len);
        uint8_t readRegister(uint8_t reg);

        void readData(float &x, float &y, float &z, float &t);
        void readRawData(uint32_t &x, uint32_t &y, uint32_t &z, uint8_t &t);
        void set();
        void reset();

        enum MBW:uint8_t{
            MBW_100Hz,
            MBW_200Hz,
            MBW_400Hz,
            MBW_800Hz
        };

        enum CM_Freq:uint8_t{
            CM_OFF,
            CM_1Hz,
            CM_10Hz,
            CM_20Hz,
            CM_50Hz,
            CM_100Hz,
            CM_200Hz,
            CM_1000Hz
        };

        enum MSET:uint8_t{
            MSET_1,
            MSET_25,
            MSET_70,
            MSET_100,
            MSET_250,
            MSET_500,
            MSET_1000,
            MSET_2000
        };

        static constexpr float mag_res = 1.f/16384.f; // assuming 18bit operation
        static constexpr int32_t mag_offset = 131072;

        static constexpr float Temp_Factor = 0.8; //0.8c/lsb
        static constexpr float Temp_Offset = 75;

        static constexpr uint8_t RW = 0x80;

        //registers
        static constexpr uint8_t WHO_AM_I = 0x2F;
        static constexpr uint8_t WHO_AM_I_RES = 0x30; 
        static constexpr uint8_t I2C_ADDRESS = 0x30;


        static constexpr uint8_t XOUT_0 = 0x00;
        static constexpr uint8_t XOUT_1 = 0x01;
        static constexpr uint8_t YOUT_0 = 0x02;
        static constexpr uint8_t YOUT_1 = 0x03;
        static constexpr uint8_t ZOUT_0 = 0x04;
        static constexpr uint8_t ZOUT_1 = 0x05;
        static constexpr uint8_t XYZOUT_2 = 0x06;
        static constexpr uint8_t TOUT = 0x07;
        static constexpr uint8_t STATUS = 0x08;
        static constexpr uint8_t CTRL_0 = 0x09;
        static constexpr uint8_t MAG_SET = 0b00010000; // degauses internal amr
        static constexpr uint8_t MAG_RESET = 0b00001000;
        static constexpr uint8_t AUTO_SR = 0b00100000;

        static constexpr uint8_t CTRL_1 = 0x0A;
        static constexpr uint8_t RESET = 0b10000000;

        static constexpr uint8_t CTRL_2 = 0x0B;
        static constexpr uint8_t CM_EN = 0b00001000;
        static constexpr uint8_t MSET_EN = 0b10000000;

        static constexpr uint8_t CTRL_3 = 0x0C;

};