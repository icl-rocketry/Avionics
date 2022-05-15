#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"




class MS5607{
    public:
        MS5607(SPIClass& spi,SystemStatus& systemstatus,LogController& logcontroller,uint8_t cs);
        void setup();
        void update(SensorStructs::BARO_t& barodata);

        /**
         * @brief zero barometer altitude 
         * 
         */
        void calibrateBaro();


    private:

        float refTemp{273.15 + 15};
        float refPress{101325};
        //pointer to spi object
        SPIClass& _spi;
        //pointer to system status object
        SystemStatus& _systemstatus;
        LogController& _logcontroller;
        const uint8_t _cs;


       

        enum BARO_CONVERSION{
            TEMPERATURE,
            PRESSURE
        };

        struct baro_cal_t{
            uint16_t pressure_sensitivity;            
            uint16_t pressure_offset;            
            uint16_t temp_coef_pressure_sensitivity;           
            uint16_t temp_coef_pressure_offset;           
            uint16_t ref_temp;            
            uint16_t temp_coef_temp;            
        }; 
        baro_cal_t calibration;

        enum BARO_CMD {
            MS5607_RESET                = 0x1E,
            MS5607_CONVERT_D1           = 0x40,
            MS5607_CONVERT_D2           = 0x50,
            MS5607_ADC_READ             = 0x00,
            MS5607_PROM_C1              = 0xA2,
            MS5607_PROM_C2              = 0xA4,
            MS5607_PROM_C3              = 0xA6,
            MS5607_PROM_C4              = 0xA8,
            MS5607_PROM_C5              = 0xAA,
            MS5607_PROM_C6              = 0xAC
        };

        enum BARO_OSR {
            MS5607_OSR256   = 0x00,
            MS5607_OSR512   = 0x02, 
            MS5607_OSR1024  = 0x04,
            MS5607_OSR2048  = 0x06,
            MS5607_OSR4096  = 0x08,
        };

        

        uint32_t D1 {0};
        uint32_t D2 {0};
        int32_t dT {0};
        int32_t TEMP {0};
        int64_t OFF {0};
        int64_t SENS {0};
        int32_t PRESS {0};

        uint8_t oversamplingRate {};
        uint32_t osrdelay {0};

        bool _conversionInProgress = false;
        uint64_t _conversionTimeStarted = 0;
        uint8_t _currentConversion = BARO_CONVERSION::TEMPERATURE;


        void reset();
        void setOversamplingRate(const BARO_OSR osr);
        void setDelay();
        void readProm();

        // bool startTemperatureConversion();
        // bool startPressureConversion();

        bool getRawTemp();
        bool getRawPressure();

        void compensateSecondOrder();
        
        float toAltitude(float pressure);

        void write(const int command, const int ms = 0);
        uint16_t read16(uint8_t command);
        uint32_t read24(uint8_t command);

        bool calculatePressure();
        bool calculateTemperature();
};    

