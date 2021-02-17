#ifndef BARO_H
#define BARO_H
#include <Arduino.h>
#include "SPI.h"

#include "Logging/systemstatus.h"

#include "sensorStructs.h"

struct baro_data_t {
    uint16_t temp;
    uint16_t pressure;
};

class Baro{
    public:
        Baro(SPIClass* spi,SystemStatus* systemstatus,raw_measurements_t* raw_data);
        void setup();
        void update();

    private:
        //pointer to spi object
        SPIClass* _spi;
        //pointer to system status object
        SystemStatus* _systemstatus;
        //pointer to raw data struct
        raw_measurements_t* _raw_data;

        int32_t D1 {0};
        int32_t D2 {0};
        int64_t dT {0};
        int32_t TEMP {0};
        int64_t OFF {0};
        int64_t SENS {0};
        int32_t PRESS {0};
        uint8_t oversamplingRate {MS5607_OSR};
        uint32_t osrdelay {1000};
        uint32_t lastConversion;

        typedef struct {
            uint16_t pressure_sensitivity {};            
            uint16_t pressure_offset {};            
            uint16_t temp_coef_pressure_sensitivity {};           
            uint16_t temp_coef_pressure_offset {};           
            uint16_t ref_temp {};            
            uint16_t temp_coef_temp {};            
        } BARO_CAL;

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

        int32_t D1 {0};
        int32_t D2 {0};
        int64_t dT {0};
        int32_t TEMP {0};
        int64_t OFF {0};
        int64_t SENS {0};
        int32_t PRESS {0};
        uint8_t oversamplingRate {MS5607_OSR4096};
        uint32_t osrdelay {9250};
        uint32_t lastConversion;


        void reset();
        void setOversamplingRate(uint8_t rate);
        void readProm();
        void setDelay();

        bool startTemperatureConversion();
        bool startPressureConversion();

        void compensateSecondOrder();

        void write8(uint8_t command);
        uint8_t read16(uint8_t command);
        uint8_t read24(uint8_t command);






};    

#endif
