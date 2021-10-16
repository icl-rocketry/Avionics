#include "baro.h"
#include "config.h"
#include "ricardo_pins.h"
#include "SPI.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"

#include <string>


Baro::Baro(SPIClass* spi,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data):
_spi(spi),
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_raw_data(raw_data)
{};

void Baro::setup(){

    reset();
    setOversamplingRate(MS5607_OSR4096);
    readProm();
    
}

void Baro::update(){
    

    getRawTemp();
    getRawPressure();

    calculateTemperature();
    calculatePressure();

    updateData();  


}


void Baro::zero(float temp,float press) 
{
    refTemp = 273.17+_raw_data->baro_temp;
    refPress = _raw_data->baro_press;
}

void Baro::reset() {
    write(BARO_CMD::MS5607_RESET, 3);
}


void Baro::readProm() {
    calibration.pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C1);
    calibration.pressure_offset = read16(BARO_CMD::MS5607_PROM_C2);
    calibration.temp_coef_pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C3);
    calibration.temp_coef_pressure_offset = read16(BARO_CMD::MS5607_PROM_C4);
    calibration.ref_temp = read16(BARO_CMD::MS5607_PROM_C5);
    calibration.temp_coef_temp = read16(BARO_CMD::MS5607_PROM_C6);
}

void Baro::setOversamplingRate(const BARO_OSR osr){
    oversamplingRate = osr;
    switch (osr)
    {
    case MS5607_OSR256:
        {
        osrdelay =  600; //750;
        break;
        }
    case MS5607_OSR512:
        {
        osrdelay = 1170; //1250;
        break;
        }
    case MS5607_OSR1024:
        {
        osrdelay = 2280; //2500;
        break;
        }
    case MS5607_OSR2048:
        {
        osrdelay = 4540; //4750;
        break;
        }
    case MS5607_OSR4096:
        {
        osrdelay = 9040; //9250;
        break;
        }
    }
}

bool Baro::getRawTemp() {
    if (_currentConversion == BARO_CONVERSION::TEMPERATURE){
        if (!_conversionInProgress){
            write(BARO_CMD::MS5607_CONVERT_D2 | oversamplingRate,0); // write spi command to start conversion
            _conversionTimeStarted = micros();
            _conversionInProgress = true;
            return true; // return out of function for non blocking
        }else{

            if (micros() - _conversionTimeStarted > osrdelay){
                uint32_t newD2 = read24(BARO_CMD::MS5607_ADC_READ);
                D2 = (newD2 == 0 ? D2:newD2); // check we havent got bad readings
                _conversionInProgress = false;
                _currentConversion = BARO_CONVERSION::PRESSURE; // set so that raw pressure is read next
                return true;
            }
        }
    }
    return true;
}


bool Baro::getRawPressure(){
    if (_currentConversion == BARO_CONVERSION::PRESSURE){
        if (!_conversionInProgress){
            write(BARO_CMD::MS5607_CONVERT_D1 | oversamplingRate,0); // write spi command to start conversion
            _conversionTimeStarted = micros();
            _conversionInProgress = true;
            return true; // return out of function for non blocking
        }else{

            if (micros() - _conversionTimeStarted > osrdelay){
                uint32_t newD1 = read24(BARO_CMD::MS5607_ADC_READ);
                D1 = (newD1 == 0 ? D1:newD1); // check the result makes sense
                _conversionInProgress = false;
                _currentConversion = BARO_CONVERSION::TEMPERATURE; // set so that raw pressure is read next
                return true;
            }
        }
    }
    return true;
}

void Baro::compensateSecondOrder() {
    int64_t T2 = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    // Low Temperature
    if (TEMP < 2000){
        int64_t TEMP_SQ = ( (int64_t)(TEMP-2000) )*( (int64_t)(TEMP-2000) );

        T2 = ( ((int64_t)dT * (int64_t)dT) >> 31);                      
        OFF2 = ((61 * TEMP_SQ) >> 4 );       
        SENS2 = (2 * TEMP_SQ);            

        // Very Low Temperature
        if (TEMP < -1500) {
            int64_t TEMP_SQ2 = ( ((int64_t)(TEMP + 1500))*((int64_t)(TEMP + 1500)) );
            OFF2 += (int64_t)15 * (TEMP_SQ2);       
            SENS2 += (int64_t)8 * (TEMP_SQ2);       
        }
        TEMP = TEMP - T2;
        OFF = OFF - OFF2;
        SENS = SENS - SENS2;
    }   
}

bool Baro::calculatePressure() {

                        
    OFF = ((int64_t)calibration.pressure_offset << 17) + (((int64_t)dT * (int64_t)calibration.temp_coef_pressure_offset) >> 6);    
    if (OFF > 25769410560) OFF = 25769410560;
    if (OFF < -17179344900) OFF = -17179344900;

    SENS = ((int64_t)calibration.pressure_sensitivity << 16) + ((int64_t)(dT * (int64_t)calibration.temp_coef_pressure_sensitivity) >> 7);  
    if (SENS > 12884705280) SENS = 12884705280;
    if (SENS < -8589672450) SENS = -8589672450;

    compensateSecondOrder();
    PRESS = ((((int64_t)D1 * SENS) >> 21) - OFF) >> 15;              
    return true;
}

bool Baro::calculateTemperature() {

    // dT = D2 - (calibration.ref_temp << 8);
    // TEMP = 2000 + ((int64_t)(dT*calibration.temp_coef_temp) >> 23);
    // return true;

    dT = (int32_t)D2 - ((int32_t)calibration.ref_temp << 8);
    if (dT > 16777216) dT = 16777216;
    if (dT < -16776960) dT = -16776960;

    TEMP = 2000 + (((int64_t)dT*calibration.temp_coef_temp) >> 23);
    return true;

}

void Baro::updateData() {
    _raw_data->baro_temp = (float)TEMP/100.0;
    _raw_data->baro_press = (float)PRESS; // leave as is for pascals
    _raw_data->baro_alt = toAltitude(PRESS);
}

void Baro::write(const int command, const int ms){
    digitalWrite(BaroCs, LOW);
    _spi->transfer(command);
    if (ms) delay(ms);
    digitalWrite(BaroCs, HIGH);
}

uint16_t Baro::read16(const uint8_t command) {
    digitalWrite(BaroCs, LOW);
    uint16_t data;
    _spi->transfer(command);
    int data1 = _spi->transfer(0x00);
    int data2 = _spi->transfer(0x00);
    data = data1 << 8 | data2;
    digitalWrite(BaroCs, HIGH);
    return data;
}

uint32_t Baro::read24(const uint8_t command) {


    digitalWrite(BaroCs, LOW);
    uint32_t data;
    _spi->transfer(command);
    int data1 = _spi->transfer(0x00);
    int data2 = _spi->transfer(0x00);
    int data3 = _spi->transfer(0x00);
    data  = data1 << 16 | data2 << 8 | data3;
    digitalWrite(BaroCs, HIGH);
    return data;    
}

float Baro::toAltitude(float pressure) {

    constexpr float R = 287.052; // specific gas constant R*/M0
    constexpr float g = 9.80665; // standard gravity 
    constexpr float t_grad = 0.0065; // gradient of temperature

    return refTemp / t_grad * (1 - exp((t_grad * R / g) * log(pressure / refPress)));
}
