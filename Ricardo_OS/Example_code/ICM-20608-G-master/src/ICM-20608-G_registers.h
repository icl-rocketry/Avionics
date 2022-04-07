#pragma once

#define GYRO_CONFIG 0x1B
#define DPS250 0b00000000
#define DPS500 0b00001000
#define DPS1000 0b00010000
#define DPS2000 0b00011000

#define ACCEL_CONFIG 0x1C
#define G2 0b00000000
#define G4 0b00001000
#define G8 0b00010000
#define G16 0b00011000

#define USER_CTRL 0x6A
#define I2C_IF_DIS 0b00010000

#define PWR_MGMT_1 0x6B
#define SLEEP 0b01000000 // is set by default
#define RESET 0b01000000
#define CLK_ZGYRO 0b00000011

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define XG_OFFS_USRH 0x13
#define XG_OFFS_USRL 0x14
#define YG_OFFS_USRH 0x15
#define YG_OFFS_USRL 0x16
#define ZG_OFFS_USRH 0x17
#define ZG_OFFS_USRL 0x18
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E

#define UNDOC1 0x11
#define UNDOC1_VALUE 0xc9