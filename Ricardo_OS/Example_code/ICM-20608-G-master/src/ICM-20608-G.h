#pragma once

#include <stdint.h>
#include <SPI.h>

#define CS D10

enum GyroRange
{
    G_250_DEGS,
    G_500_DEGS,
    G_1000_DEGS,
    G_2000_DEGS
};

enum AccelRange
{
    A_2_G,
    A_4_G,
    A_8_G,
    A_16_G
};

class ICM_20608_G
{
public:
    ICM_20608_G(GyroRange g, AccelRange a);

    int8_t init();

    void calibrate();

    void writeRegister(uint8_t reg, uint8_t val);

    uint8_t readRegister(uint8_t reg);

    void readGyro(float &x, float &y, float &z) const;

    void readAccel(float &x, float &y, float &z) const;

    int16_t offset_gx;
    int16_t offset_gy;
    int16_t offset_gz;
    int16_t offset_ax;
    int16_t offset_ay;
    int16_t offset_az;

private:
    SPISettings settings;

    GyroRange gyro_range;

    AccelRange accel_range;

    float gyro_lsb_to_degs;
    float accel_lsb_to_g;

    void readGyroRaw(int16_t &x, int16_t &y, int16_t &z) const;
    void readAccelRaw(int16_t &x, int16_t &y, int16_t &z) const;
};