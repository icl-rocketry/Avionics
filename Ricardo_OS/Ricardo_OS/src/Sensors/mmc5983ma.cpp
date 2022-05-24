#include "mmc5983ma.h"

#include <SPI.h>

#include "config.h"

#include "sensorStructs.h"
#include "Storage/logController.h"
#include "Storage/systemstatus.h"

#include <Eigen/Core>

#include <Preferences.h>

MMC5983MA::MMC5983MA(SPIClass &spi, SystemStatus &systemstatus, LogController &logcontroller, uint8_t cs) : _spi(spi),
                                                                                                            _systemstatus(systemstatus),
                                                                                                            _logcontroller(logcontroller),
                                                                                                            _cs(cs), // update with correct chip select
                                                                                                            _magCal{1,
                                                                                                                    0,
                                                                                                                    0,
                                                                                                                    Eigen::Matrix3f{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
                                                                                                                    Eigen::Vector3f{{0, 0, 0}}} // default for mag biases
                                                                                                            {};

void MMC5983MA::setup(const std::array<uint8_t,3>& axesOrder,const std::array<bool,3>& axesFlip)
{
    // reset chip
    writeRegister(CTRL_1, RESET);
    set(); // reset coils
    reset();
    // set control registers
    //enable auto_set_reset
    writeRegister(CTRL_0,AUTO_SR);
    //set bandwidth
    writeRegister(CTRL_1,MBW_100Hz);
    //enable continous measurement and enable periodic set/reset
    writeRegister(CTRL_2, CM_100Hz | 0x08 | (MSET_1000 << 4) | 0x80);

    loadMagCal(); // load calibration from nvs

    if (!alive())
    {
        _systemstatus.new_message(SYSTEM_FLAG::ERROR_MAG, "Unable to initialize the MMC5983MA");
        return;
    }

    axeshelper.setOrder(axesOrder);
    axeshelper.setFlip(axesFlip);

    _logcontroller.log("MMC5983MA Initialized");

}

void MMC5983MA::update(SensorStructs::MAG_3AXIS_t& data)
{
    float raw_mx;
    float raw_my;
    float raw_mz;
    readData(raw_mx, raw_my, raw_mz, data.temp);

    std::array<float,3> mag_transformed = axeshelper(std::array<float,3>{-raw_my,-raw_mx,raw_mz});
    //apply calibration
    Eigen::Vector3f corrected_mag = _magCal.A_1 * (Eigen::Vector3f{mag_transformed[0],mag_transformed[1],mag_transformed[2]} - _magCal.b);
    data.mx = corrected_mag[0];
    data.my = corrected_mag[1];
    data.mz = corrected_mag[2];
};

bool MMC5983MA::alive(){
    return (readRegister(WHO_AM_I) == WHO_AM_I_RES);
}


void MMC5983MA::readData(float &x, float &y, float &z, float &t)
{
    uint32_t raw_x;
    uint32_t raw_y;
    uint32_t raw_z;
    uint8_t raw_temp;

    readRawData(raw_x,raw_y,raw_z,raw_temp);

    x = (float)((int32_t)raw_x - mag_offset) * mag_res;
    y = (float)((int32_t)raw_y - mag_offset) * mag_res;
    z = (float)((int32_t)raw_z - mag_offset) * mag_res;

    t = ((float)raw_temp * Temp_Factor) - Temp_Offset;
}

void MMC5983MA::readRawData(uint32_t &x, uint32_t &y, uint32_t &z, uint8_t &t)
{
    uint8_t rawData[8];  // x/y/z/t mag register data stored here
    readRegister(XOUT_0, &rawData[0],8);  // Read the 8 raw data registers into data array
    x = (uint32_t)(rawData[0] << 10 | rawData[1] << 2 | (rawData[6] & 0xC0) >> 6); // Turn the 18 bits into a unsigned 32-bit value
    y = (uint32_t)(rawData[2] << 10 | rawData[3] << 2 | (rawData[6] & 0x30) >> 4); // Turn the 18 bits into a unsigned 32-bit value
    z = (uint32_t)(rawData[4] << 10 | rawData[5] << 2 | (rawData[6] & 0x0C) >> 2); // Turn the 18 bits into a unsigned 32-bit value
    t = rawData[7];
}


void MMC5983MA::writeRegister(uint8_t reg_address, uint8_t data)

{
    // SPI write handling code
    digitalWrite(_cs, LOW);
    _spi.transfer(reg_address); 
    _spi.transfer(data);
    digitalWrite(_cs, HIGH);
  
}

void MMC5983MA::set()
{
    writeRegister(CTRL_0,MAG_SET);
    delay(5);
}

void MMC5983MA::reset()
{
    writeRegister(CTRL_0,MAG_RESET);
    delay(5);
}

void MMC5983MA::readRegister(uint8_t reg_address, uint8_t *data, uint8_t len)

{
    // SPI read handling code
    digitalWrite(_cs, LOW);
    _spi.transfer(reg_address | RW); // 0b10000000
    for (int i = 0; i < len; i++)
    {
        data[i] = _spi.transfer(0);
    }
    digitalWrite(_cs, HIGH);
}

uint8_t MMC5983MA::readRegister(uint8_t reg){
    uint8_t data;
    readRegister(reg,&data,1);
    return data;
}

void MMC5983MA::writeMagCal() 
{
    Preferences pref;

    if (!pref.begin("MAG")){
        _logcontroller.log("nvs failed to start. Can't write calbration offsets");
        return;
    }   

    if (!pref.putFloat("F",_magCal.fieldMagnitude)){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("I",_magCal.inclination)){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("D",_magCal.declination)){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A11",_magCal.A_1(0,0))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A12",_magCal.A_1(0,1))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A12",_magCal.A_1(0,2))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A21",_magCal.A_1(1,0))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A22",_magCal.A_1(1,1))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A22",_magCal.A_1(1,2))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A31",_magCal.A_1(2,0))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A32",_magCal.A_1(2,1))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("A32",_magCal.A_1(2,2))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("b1",_magCal.b(0))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("b2",_magCal.b(1))){_logcontroller.log("nvs error while writing");};
    if (!pref.putFloat("b3",_magCal.b(2))){_logcontroller.log("nvs error while writing");};
    
 

}

void MMC5983MA::loadMagCal() 
{
    Preferences pref;

    if (!pref.begin("MAG",true)){
        _logcontroller.log("nvs failed to start");
        return;
    }  

    _magCal.fieldMagnitude = pref.getFloat("F",1);
    _magCal.inclination = pref.getFloat("I",0);
    _magCal.declination = pref.getFloat("D",0);

    _magCal.A_1 << pref.getFloat("A11",1),pref.getFloat("A12",0),pref.getFloat("A13",0),
                   pref.getFloat("A21",0),pref.getFloat("A22",1),pref.getFloat("A23",0),
                   pref.getFloat("A31",0),pref.getFloat("A32",0),pref.getFloat("A33",1);

    _magCal.b << pref.getFloat("b1",0),pref.getFloat("b2",0),pref.getFloat("b3",0);
}

void MMC5983MA::calibrate(MagCalibrationParameters magCal) 
{
    _magCal = magCal;
    writeMagCal();
}
