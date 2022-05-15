#include "h3lis331dl.h"

H3LIS331DL::H3LIS331DL(SPIClass& spi,SystemStatus& systemstatus,LogController& logcontroller,uint8_t cs):
_spi(spi),
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_cs(cs)// update with correct chip select
{};

void H3LIS331DL::setup(const std::array<uint8_t,3>& axesOrder, const std::array<bool,3>& axesFlip)
{
    setPowerMode(power_mode::NORMAL);
    axesEnable(true);

    uint8_t data = 0;
    for (int i = 0x21; i < 0x25; i++) writeRegister(i,&data,1);
    for (int i = 0x30; i < 0x37; i++) writeRegister(i,&data,1);

    setODR(data_rate::DR_400HZ);
    setFullScale(fs_range::FS_100G); // needs to be changed to be configured from json

     if (!alive()){
         _systemstatus.new_message(SYSTEM_FLAG::ERROR_HACCEL, "Unable to initialize the H3LIS331DL");
        return;
    }

    axeshelper.setOrder(axesOrder);
    axeshelper.setFlip(axesFlip);

    _logcontroller.log("H3LIS331DL Initialized");

}

void H3LIS331DL::update(SensorStructs::ACCEL_3AXIS_t& data)
{
    readAxes(data.ax,data.ay,data.az);
};

bool H3LIS331DL::alive(){
    return (readRegister(WHO_AM_I) == WHO_AM_I_RES);
}

void H3LIS331DL::axesEnable(bool enable)
{
  uint8_t data;
  readRegister(CTRL_REG1, &data, 1);
  if (enable)
  {
    data |= 0x07;
  }
  else
  {
    data &= ~0x07;
  }
  writeRegister(CTRL_REG1, &data, 1);
}

void H3LIS331DL::setPowerMode(power_mode pmode)
{
  uint8_t data;
  readRegister(CTRL_REG1, &data, 1);

  // The power mode is the high three bits of CTRL_REG1. The mode 
  //  constants are the appropriate bit values left shifted by five, so we 
  //  need to right shift them to make them work. We also want to mask off the
  //  top three bits to zero, and leave the others untouched, so we *only*
  //  affect the power mode bits.
  data &= ~0xe0; // Clear the top three bits
  data |= pmode<<5; // set the top three bits to our pmode value
  writeRegister(CTRL_REG1, &data, 1); // write the new value to CTRL_REG1
}

void H3LIS331DL::setODR(data_rate drate)
{
  uint8_t data;
  readRegister(CTRL_REG1, &data, 1);

  // The data rate is bits 4:3 of CTRL_REG1. The data rate constants are the
  //  appropriate bit values; we need to right shift them by 3 to align them
  //  with the appropriate bits in the register. We also want to mask off the
  //  top three and bottom three bits, as those are unrelated to data rate and
  //  we want to only change the data rate.
  data &=~0x18;     // Clear the two data rate bits
  data |= drate<<3; // Set the two data rate bits appropriately.
  writeRegister(CTRL_REG1, &data, 1); // write the new value to CTRL_REG1
}

void H3LIS331DL::setFullScale(fs_range range)
{
    uint8_t data;
    readRegister(CTRL_REG4, &data, 1);
    data &= ~0xcf;
    data |= range << 4;
    writeRegister(CTRL_REG4, &data, 1);
    switch (range)
    {
    case fs_range::FS_100G:
    {
        raw_to_g = 100.0f / 2047.0f;
        break;
    }
    case fs_range::FS_200G:
    {
        raw_to_g = 200.0f / 2047.0f;
        break;
    }
    case fs_range::FS_400G:
    {
        raw_to_g = 400.0f / 2047.0f;
        break;
    }
        default:
        {
            raw_to_g = 0;
            // log error
            break;
        }
    }
}

void H3LIS331DL::readRawAxes(int16_t &x, int16_t &y, int16_t &z)
{
  uint8_t data[6]; // create a buffer for our incoming data
  readRegister(OUT_X_L, &data[0], 1);
  readRegister(OUT_X_H, &data[1], 1);
  readRegister(OUT_Y_L, &data[2], 1);
  readRegister(OUT_Y_H, &data[3], 1);
  readRegister(OUT_Z_L, &data[4], 1);
  readRegister(OUT_Z_H, &data[5], 1);
  // The data that comes out is 12-bit data, left justified, so the lower
  //  four bits of the data are always zero. We need to right shift by four,
  //  then typecase the upper data to an integer type so it does a signed
  //  right shift.
  x = data[0] | data[1] << 8;
  y = data[2] | data[3] << 8;
  z = data[4] | data[5] << 8;
  x = x >> 4;
  y = y >> 4;
  z = z >> 4;
}

void H3LIS331DL::readAxes(float &x, float &y, float &z)
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    readRawAxes(ax,ay,az);

    std::array<float, 3> accel = axeshelper(std::array<float, 3>{-raw_to_g * float(ay),
                                                                 -raw_to_g * float(ax),
                                                                 -raw_to_g * float(az)});

    x = accel[0];
    y = accel[1];
    z = accel[2];
}

void H3LIS331DL::writeRegister(uint8_t reg_address, uint8_t *data, uint8_t len)

{
    // SPI write handling code
    digitalWrite(_cs, LOW);
    _spi.transfer(reg_address | 0x40); // 0b01000000
    for (int i=0; i<len; i++)
    {
      _spi.transfer(data[i]);
    }
    digitalWrite(_cs, HIGH);
  
}

void H3LIS331DL::readRegister(uint8_t reg_address, uint8_t *data, uint8_t len)

{
    // SPI read handling code
    digitalWrite(_cs, LOW);
    _spi.transfer(reg_address | 0xC0); // 0b11000000
    for (int i = 0; i < len; i++)
    {
        data[i] = _spi.transfer(0);
    }
    digitalWrite(_cs, HIGH);
}

uint8_t H3LIS331DL::readRegister(uint8_t reg){
    uint8_t data;
    readRegister(reg,&data,1);
    return data;
}