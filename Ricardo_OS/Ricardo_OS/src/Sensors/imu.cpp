#include "imu.h"
#include "config.h"
#include "SPI.h"

#include "Adafruit_Sensor.h"
#include "ricardo_pins.h"
#include "../Logging/messages.h"
#include "SparkFunLSM9DS1.h"



#ifdef RUAIRIDH_TEST_BOARD
    //intermediate definitions: will move to config later
    #define LSM9DS1_M_CS	10 
    #define LSM9DS1_AG_CS	9  
    #define LSM9DS1_SCK  18
    #define LSM9SD1_MISO 19
    #define LSM9SD1_MOSI 23

    #define ACCEL_SCALE 2
    #define MAG_SCALE 4
    #define GYRO_SCALE 245

    #define PRINT_CALCULATED
#endif

Imu::Imu(SPIClass* spi):imu(spi)
{
   // _spi = spi;
};

void Imu::setup(){
    #ifdef RUAIRIDH_TEST_BOARD
        Serial.begin(115200);
    #endif
    imu.setAccelScale(ACCEL_SCALE);
    imu.setGyroScale(GYRO_SCALE);
    imu.setMagScale(MAG_SCALE);

    if (!imu.beginSPI(LSM9DS1_SCK, LSM9SD1_MISO, LSM9SD1_MOSI, LSM9DS1_AG_CS, LSM9DS1_M_CS)){
        new_message(ERROR_IMU, "Unable to initialize the imu");
    };

};

void Imu::update(){


    #ifdef RUAIRIDH_TEST_BOARD
        printAccel();
    #endif
};

void Imu::read_gyro(){
  if(imu.gyroAvailable()){
        imu.readGyro();
        imu_data.gx = 

    }
}
void Imu::read_accel(){
  if(imu.accelAvailable()){
        imu.readAccel();

    }
}
void Imu::read_mag(){
  if(imu.magAvailable()){
        imu.readMag();
    }
}
void Imu::read_temp(){
  if(imu.tempAvailable()){
        imu.readTemp();
    }
}

void Imu::printAccel()
{

  imu.readAccel();
  
  Serial.print("A: ");
#ifdef PRINT_CALCULATED

  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.println(imu.calcAccel(imu.az), 2);
#elif defined PRINT_RAW 
  Serial.print(imu.ax);
  Serial.print(", ");
  Serial.print(imu.ay);
  Serial.print(", ");
  Serial.println(imu.az);
#endif

}
