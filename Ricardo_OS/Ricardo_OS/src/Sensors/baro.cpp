#include "baro.h"
#include "config.h"
#include "SPI.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"


Baro::Baro(SPIClass* spi,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data):
_spi(spi),
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_raw_data(raw_data)
{};

void Baro::setup(){
    //some sort of gps intilization idk
    reset();
    setDelay();
    readProm();
    
}

void Baro::update(){
    
    if (_spi != NULL){ //check for null pointer to rpevent undfiend behaviour
        getRawPressure();
        getRawTemperature();

        readTemperature();
        readPressure();

        export();  
    };
}

void Baro::reset() {
    Baro::write(BARO_CMD::MS5607_RESET, 3);
}

void Baro::setOversamplingRate(uint8_t rate){
    oversamplingRate = rate;
}

void Baro::readProm() {
    calibration.pressure_offset = read16(BARO_CMD::MS5607_PROM_C1);
    calibration.pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C2);
    calibration.temp_coef_pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C3);
    calibration.temp_coef_pressure_offset = read16(BARO_CMD::MS5607_PROM_C4);
    calibration.ref_temp = read16(BARO_CMD::MS5607_PROM_C5);
    calibration.temp_coef_temp = read16(BARO_CMD::MS5607_PROM_C6);
}

void Baro::setDelay(){
    switch (oversamplingRate)
    {
    case 0:
        osrdelay =  600; //750;
        break;
    case 2:
        osrdelay = 1100; //1250;
        break;
    case 4:
        osrdelay = 2250; //2500;
        break;
    case 6:
        osrdelay = 4300; //4750;
        break;
    case 8:
        osrdelay = 9040; //9250;
        break;
    }
}

bool Baro::startTemperatureConversion() {
    D2 = read24(BARO_CMD::MS5607_CONVERT_D2 | oversamplingRate);
    lastConversion = micros();
    return true;
}

bool Baro::startPressureConversion() {
    D1 = read24(BARO_CMD::MS5607_CONVERT_D1 | oversamplingRate);
    lastConversion =  micros();
    return true;
}

void Baro::compensateSecondOrder() {
    int32_t T2 = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    // Low Temperature
    if (TEMP < 2000){
        T2 = ((dT * dT) >> 31);                      
        OFF2 = 61 * (TEMP - 2000)*(TEMP - 2000) >> 4;       
        SENS2 = 2 * (TEMP - 2000)*(TEMP - 2000);            

        // Very Low Temperature
        if (TEMP < -1500) {
            OFF2 += 15 * (TEMP + 1500)*(TEMP + 1500);       
            SENS2 += 8 * (TEMP + 1500)*(TEMP + 1500);       
        }
        TEMP = TEMP - T2;
        OFF = OFF - OFF2;
        SENS = SENS - SENS2;
    }   
}

bool Baro::readPressure() {
    if (micros() - lastConversion > osrdelay){                          
        OFF = ((int64_t)calibration.pressure_offset << 17) + ((dT * calibration.temp_coef_pressure_offset) >> 6);    
        SENS = ((int64_t)calibration.pressure_sensitivity << 16) + ((dT * calibration.temp_coef_pressure_sensitivity) >> 7);  

        compensateSecondOrder();
        PRESS = (((D1 * SENS) >> 21) - OFF) >> 15;              
        return true;
    } else {
        return false;
        }
}

bool Baro::readTemperature() {
    if (micros() - lastConversion > osrdelay) {
        dT = D1 - ((uint32_t)calibration.ref_temp << 8);
        TEMP = 2000 + ((dT*(int64_t)calibration.temp_coef_temp) >> 23);
        return true;
    } else {
        return false;
        }
}

void Baro::export() {
    _raw_data->baro_temp = TEMP/100;
    _raw_data->baro_press = PRESS/100;
    _raw_data->baro_alt = toAltitude(PRESS/100);
}

void Baro::write(const int command, const int ms = 0){
    digitalWrite(MS5607_CS, LOW);
    _spi->transfer(command);
    if (ms) {
        delay(ms);
    }
    digitalWrite(MS5607_CS, HIGH);
}

uint16_t Baro::read16(const uint8_t command) {
    digitalWrite(MS5607_CS, LOW);
    uint16_t data {};
    _spi->transfer(command);
    int data1 = _spi->transfer(0x00);
    int data2 = _spi->transfer(0x00);
    data = data1 << 8 | data2;
    digitalWrite(MS5607_CS, HIGH);
    return data;
}

uint32_t Baro::read24(const uint8_t command) {
    digitalWrite(MS5607_CS, LOW);
    _spi->transfer(command);
    digitalWrite(MS5607_CS, HIGH);

    delayMicroseconds(osrdelay);

    digitalWrite(MS5607_CS, LOW);
    uint32_t data {};
    _spi->transfer(BARO_CMD::MS5607_ADC_READ);
    int data1 = _spi->transfer(0x00);
    int data2 = _spi->transfer(0x00);
    int data3 = _spi->transfer(0x00);
    data  = data1 << 16 | data2 << 8 | data3;
    digitalWrite(MS5607_CS, HIGH);
    return data;    
}

float toAltitude(float pressure) {

    const float R = 287.052; // specific gas constant R*/M0
    const float g = 9.80665; // standard gravity 
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude
    const float p0 = 101325; // pressure at 0 altitude

    return t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / p0)));
}
