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
    setOversamplingRate(MS5607_OSR1024);
    readProm();
    
}

void Baro::update(){
    

    getRawTemp();
    getRawPressure();

    calculateTemperature();
    calculatePressure();

    updateData();  

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
    _logcontroller->log("pres_sen " + std::to_string(calibration.pressure_sensitivity) +
                        "\npres_off" + std::to_string(calibration.pressure_offset) +
                        "\ntemp_coef_pres_sens" + std::to_string(calibration.temp_coef_pressure_sensitivity) +
                        "\ntemp_coef_pres_off" + std::to_string(calibration.temp_coef_pressure_offset) +
                        "\nref_temp" + std::to_string(calibration.ref_temp) +
                        "\ntemp_coef" + std::to_string(calibration.temp_coef_temp));
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
        osrdelay = 1100; //1250;
        break;
        }
    case MS5607_OSR1024:
        {
        osrdelay = 2250; //2500;
        break;
        }
    case MS5607_OSR2048:
        {
        osrdelay = 4300; //4750;
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
                D2 = read24(BARO_CMD::MS5607_ADC_READ);
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
                D1 = read24(BARO_CMD::MS5607_ADC_READ);
                _conversionInProgress = false;
                _currentConversion = BARO_CONVERSION::TEMPERATURE; // set so that raw pressure is read next
                return true;
            }
        }
    }
    return true;
}

void Baro::compensateSecondOrder() {
    int32_t T2 = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    // Low Temperature
    if (TEMP < 2000){
        T2 = ((int64_t)(dT * dT) >> 31);                      
        OFF2 = (int64_t)61 * ((TEMP - 2000)*(TEMP - 2000)) >> 4;       
        SENS2 = (int64_t)2 * ((TEMP - 2000)*(TEMP - 2000));            

        // Very Low Temperature
        if (TEMP < -1500) {
            OFF2 += (int64_t)15 * ((TEMP + 1500)*(TEMP + 1500));       
            SENS2 += (int64_t)8 * ((TEMP + 1500)*(TEMP + 1500));       
        }
        TEMP = TEMP - T2;
        OFF = OFF - OFF2;
        SENS = SENS - SENS2;
    }   
}

bool Baro::calculatePressure() {

                        
    OFF = ((int64_t)calibration.pressure_offset << 17) + (((int64_t)dT * calibration.temp_coef_pressure_offset) >> 6);    
    SENS = ((int64_t)calibration.pressure_sensitivity << 16) + (((int64_t)dT * calibration.temp_coef_pressure_sensitivity) >> 7);  

    compensateSecondOrder();
    PRESS = ((((int64_t)D1 * SENS) >> 21) - OFF) >> 15;              
    return true;
}

bool Baro::calculateTemperature() {

    dT = D2 - (calibration.ref_temp << 8);
    TEMP = 2000 + ((dT*calibration.temp_coef_temp) >> 23);
    return true;
}

void Baro::updateData() {
    _raw_data->baro_temp = TEMP/100.0;
    _raw_data->baro_press = PRESS; // leave as is for pascals
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
    // digitalWrite(BaroCs, LOW);
    // _spi->transfer(command);
    // digitalWrite(BaroCs, HIGH);

    // delayMicroseconds(osrdelay);

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
    constexpr float t0 = 273.15 + 15; // temperature at 0 altitude
    constexpr float p0 = 101325; // pressure at 0 altitude

    return t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / p0)));
}
