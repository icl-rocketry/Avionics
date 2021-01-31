#include "baro.h"
#include "config.h"
#include "SPI.h"

#include "Logging/systemstatus.h"

#include "sensorStructs.h"


Baro::Baro(SPIClass* spi,SystemStatus* systemstatus,raw_measurements_t* raw_data):
_spi(spi),
_systemstatus(systemstatus),
_raw_data(raw_data)
{};

void Baro::setup(){
    //some sort of gps intilization idk
    reset();
    
}

void Baro::update(){
    
    if (_spi != NULL){ //check for null pointer to rpevent undfiend behaviour
  
    };
}

void Baro::reset() {
    write8(BARO_CMD::MS5607_RESET);
}

void Baro::print_calibration() {
    Serial.print("C1: ");
    Serial.println(calibration.pressure_offset);
    Serial.print("C2: ");
    Serial.println(calibration.pressure_sensitivity);
    Serial.print("C3: ");
    Serial.println(calibration.temp_coef_pressure_sensitivity);
    Serial.print("C4: ");
    Serial.println(calibration.temp_coef_pressure_offset);
    Serial.print("C5: ");
    Serial.println(calibration.ref_temp);
    Serial.print("C6: ");
    Serial.println(calibration.temp_coef_temp);
}


void Baro::readProm() {
    calibration.pressure_offset = read16(BARO_CMD::MS5607_PROM_C1);
    calibration.pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C2);
    calibration.temp_coef_pressure_sensitivity = read16(BARO_CMD::MS5607_PROM_C3);
    calibration.temp_coef_pressure_offset = read16(BARO_CMD::MS5607_PROM_C4);
    calibration.ref_temp = read16(BARO_CMD::MS5607_PROM_C5);
    calibration.temp_coef_temp = read16(BARO_CMD::MS5607_PROM_C6);
}

uint16_t Baro::read16(const BARO_CMD command) {
    uint16_t data {};
    _spi->transfer(command);
    data = _spi->transfer(0x00);
    data <<= 8;
    data |= _spi->transfer(0x00);
    return data;
}

void Baro::write8(const BARO_CMD command){
    digitalWrite(MS5607_CS, LOW);
    _spi->transfer(command);
    digitalWrite(MS5607_CS, HIGH);
}
