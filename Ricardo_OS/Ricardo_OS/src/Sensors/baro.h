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
        void reset();

        void write8(BARO_CMD command);
        
        uint16_t read16(BARO_CMD command);

        void readProm();





};    

#endif
