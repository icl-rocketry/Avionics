/*
 * Copyright (c) 2020 Tlera Corp.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimers.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimers in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Tlera Corp, nor the names of its contributors
 *     may be used to endorse or promote products derived from this Software
 *     without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 * 
 * The MMC5983MA is an AEC-Q100 qualified complete 3-axis magnetic sensor with on-chip 
 * signal processing and integrated I2C/SPI bus suitable for use in automotive applications.
 * Couple with the LSM6DSM accel/gyro and LPS22HB barometer the system provides a compact and
 * accurate absolute orientation estimation engine.
 * 
 * Uses the I2C interface for data transfer.
 * 
 * Intended to be run on a Dragonfly development board:
 * 
 * https://www.tindie.com/products/tleracorp/dragonfly-stm32l47696-development-board/
 *  
 * 
 * Library may be used freely and without limit with attribution.
 */

#include "MMC5983MA.h"
#include "I2Cdev.h"

MMC5983MA::MMC5983MA(I2Cdev* i2c_bus)
{
  _i2c_bus = i2c_bus;
}


uint8_t MMC5983MA::getChipID()
{
  uint8_t c = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_PRODUCT_ID);
  return c;
}


void MMC5983MA::reset()
{
  // reset device
  _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_1, 0x80); // Set bit 7 to 1 to reset MMC5983MA
  delay(10); // Wait 10 ms for all registers to reset 
}


void MMC5983MA::init(uint8_t MODR, uint8_t MBW, uint8_t MSET)
{
 // enable data ready interrupt (bit2 == 1), enable auto set/reset (bit 5 == 1)
 // this set/reset is a low current sensor offset measurement for normal use
 _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x20 | 0x04);  
 
 // set magnetometer bandwidth
 _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_1, MBW);  

// enable continuous measurement mode (bit 3 == 1), set sample rate
 // enable automatic Set/Reset (bit 7 == 1), set set/reset rate
 // this set/reset is a high-current "deGaussing" that should be used only to recover from 
 // high magnetic field detuning of the magnetoresistive film
// _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2, 0x80 | (MSET << 4) | 0x08 | MODR);  
 _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2, 0x08 | MODR);  
}


void MMC5983MA::selfTest()
{
    uint8_t rawData[6] = {0};  // x/y/z mag register data stored here
    uint16_t data_set[3] ={0}, data_reset[3] = {0};
    uint32_t delta_data[3] = {0};
    
   // clear control registers
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x00);  
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_1, 0x00);  
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2, 0x00);

   SET(); // enable set current
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x01);  //enable one-time mag measurement
   delay(10);
   
   _i2c_bus->readBytes(MMC5983MA_ADDRESS, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
   data_set[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
   data_set[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
   data_set[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis

   RESET(); // enable reset current
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x01);  //enable one-time mag measurement
   delay(10);
   
   _i2c_bus->readBytes(MMC5983MA_ADDRESS, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
   data_reset[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
   data_reset[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
   data_reset[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis
 
   for (uint8_t ii = 0; ii < 3; ii++)
   {
    if(data_set[ii] > data_reset[ii]) 
    { 
      delta_data[ii] = data_set[ii] - data_reset[ii];
    }
    else
    {
       delta_data[ii] = data_reset[ii] - data_set[ii];
    }
  }
  
  Serial.print("x-axis self test = "); Serial.print(delta_data[0]); Serial.println(", should be >100");
  Serial.print("y-axis self test = "); Serial.print(delta_data[1]); Serial.println(", should be >100");
  Serial.print("z-axis self test = "); Serial.print(delta_data[2]); Serial.println(", should be >100");
  }


  void MMC5983MA::getOffset(float * destination)
{
   uint8_t rawData[6] = {0};  // x/y/z mag register data stored here
   uint16_t data_set[3] ={0}, data_reset[3] = {0};
    
   powerDown();
 
   SET(); // enable set current
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x01);  //enable one-time mag measurement
   delay(10);
   
   _i2c_bus->readBytes(MMC5983MA_ADDRESS, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
   data_set[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
   data_set[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
   data_set[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis

   RESET(); // enable reset current
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x01);  //enable one-time mag measurement
   delay(10);
   
   _i2c_bus->readBytes(MMC5983MA_ADDRESS, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
   data_reset[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
   data_reset[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
   data_reset[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis
 
   for (uint8_t ii = 0; ii < 3; ii++)
   {
      destination[ii] = ((float)data_set[ii] + (float)data_reset[ii])/2.0f;
   }
}


void MMC5983MA::SET()
{
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x08);  
   delay(1); // self clearing after 500 us
}


void MMC5983MA::RESET()
{
   _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, 0x10);  
   delay(1); // self clearing after 500 us
}


uint8_t MMC5983MA::status()
{
  // Read status register
  uint8_t temp = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_STATUS);   
  return temp;
}


void MMC5983MA::clearInt()
{
  // Clear data ready interrupts
  uint8_t temp = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_STATUS);   
  _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_STATUS, temp & 0x01);   
}


void MMC5983MA::readData(uint32_t * destination)  
{
  uint8_t rawData[7];  // x/y/z mag register data stored here
  _i2c_bus->readBytes(MMC5983MA_ADDRESS, MMC5983MA_XOUT_0, 7, &rawData[0]);  // Read the 7 raw data registers into data array
  destination[0] = (uint32_t)(rawData[0] << 10 | rawData[1] << 2 | (rawData[6] & 0xC0) >> 6); // Turn the 18 bits into a unsigned 32-bit value
  destination[1] = (uint32_t)(rawData[2] << 10 | rawData[3] << 2 | (rawData[6] & 0x30) >> 4); // Turn the 18 bits into a unsigned 32-bit value
  destination[2] = (uint32_t)(rawData[4] << 10 | rawData[5] << 2 | (rawData[6] & 0x0C) >> 2); // Turn the 18 bits into a unsigned 32-bit value
}


uint8_t MMC5983MA::readTemperature()
{
  uint8_t temp = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_TOUT);  // Read the raw temperature register 
  return temp;
}


void MMC5983MA::offsetBias(float * dest1, float * dest2)
{
  int32_t mag_bias[3] = {0, 0, 0}, mag_scale[3] = {0, 0, 0};
  int32_t mag_max[3] = {-262143, -262143, -262143}, mag_min[3] = {262143, 262143, 262143};
  uint32_t mag_temp[3] = {0, 0, 0}, magOffset = 131072;
  float _mRes = 1.0f/16384.0f;        // mag sensitivity if using 18 bit data
  
  Serial.println("Calculate mag offset bias: move all around to sample the complete response surface!");
  delay(4000);

  for (int ii = 0; ii < 4000; ii++)
  {
    readData(mag_temp);
    for (int jj = 0; jj < 3; jj++) {
      if((int32_t)(mag_temp[jj] - magOffset) > mag_max[jj]) mag_max[jj] = (int32_t)(mag_temp[jj] - magOffset);
      if((int32_t)(mag_temp[jj] - magOffset) < mag_min[jj]) mag_min[jj] = (int32_t)(mag_temp[jj] - magOffset);
    }
    delay(12);
  }

    // Get hard iron correction
    mag_bias[0]  = (mag_max[0] + mag_min[0])/2;  // get average x mag bias in counts
    mag_bias[1]  = (mag_max[1] + mag_min[1])/2;  // get average y mag bias in counts
    mag_bias[2]  = (mag_max[2] + mag_min[2])/2;  // get average z mag bias in counts
    
    dest1[0] = (float) (mag_bias[0]) * _mRes;  // save mag biases in G for main program
    dest1[1] = (float) (mag_bias[1]) * _mRes;   
    dest1[2] = (float) (mag_bias[2]) * _mRes;  
       
    // Get soft iron correction estimate
    mag_scale[0]  = (mag_max[0] - mag_min[0])/2;  // get average x axis max chord length in counts
    mag_scale[1]  = (mag_max[1] - mag_min[1])/2;  // get average y axis max chord length in counts
    mag_scale[2]  = (mag_max[2] - mag_min[2])/2;  // get average z axis max chord length in counts

    float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
    avg_rad /= 3.0f;

    dest2[0] = avg_rad/((float)mag_scale[0]);
    dest2[1] = avg_rad/((float)mag_scale[1]);
    dest2[2] = avg_rad/((float)mag_scale[2]);
  
   Serial.println("Mag Calibration done!");
}


void MMC5983MA::powerDown()
{
  uint8_t temp = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2); // read register contents
  _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2, temp & ~(0x07)); // clear lowest four bits
  delay(20); // make sure to finish the lest measurement

}


void MMC5983MA::powerUp(uint8_t MODR)
{
  uint8_t temp = _i2c_bus->readByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2); // read register contents
  _i2c_bus->writeByte(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_2, temp | MODR);  // start continuous mode
}
