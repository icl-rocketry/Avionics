#include "ICM-20608-G.h"
#include "ICM-20608-G_registers.h"

ICM_20608_G::ICM_20608_G(GyroRange g, AccelRange a) : settings(8000000, MSBFIRST, SPI_MODE3), gyro_range(g), accel_range(a)
{
    switch (gyro_range)
    {
    case G_250_DEGS:
        gyro_lsb_to_degs = 250.f / 32768.f;
        break;
    case G_500_DEGS:
        gyro_lsb_to_degs = 500.f / 32768.f;
        break;
    case G_1000_DEGS:
        gyro_lsb_to_degs = 1000.f / 32768.f;
        break;
    case G_2000_DEGS:
        gyro_lsb_to_degs = 2000.f / 32768.f;
        break;
    }

    switch (accel_range)
    {
    case A_2_G:
        accel_lsb_to_g = 2.f / 32768.f;
        break;
    case A_4_G:
        accel_lsb_to_g = 4.f / 32768.f;
        break;
    case A_8_G:
        accel_lsb_to_g = 8.f / 32768.f;
        break;
    case A_16_G:
        accel_lsb_to_g = 16.f / 32768.f;
        break;
    }
}

int8_t ICM_20608_G::init()
{
    pinMode(CS, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);

    // try awaking the device 5 times
    uint8_t i;
    for (i = 1; i <= 5; i++)
    {
        writeRegister(PWR_MGMT_1, RESET);     // reset whole device
        delay(100);                           // 100ms
        writeRegister(USER_CTRL, I2C_IF_DIS); // disable I2C mode as recommended in datasheet
        writeRegister(PWR_MGMT_1, CLK_ZGYRO); // wake up
        delay(5);

        // woke up or still asleep?
        if (readRegister(PWR_MGMT_1) == CLK_ZGYRO)
        {
            // woke up, go on
            break;
        }
    }

    if (i >= 5)
    {
        return -1;
    }

    writeRegister(UNDOC1, UNDOC1_VALUE);

    switch (gyro_range)
    {
    case G_250_DEGS:
        writeRegister(GYRO_CONFIG, DPS250);
        break;
    case G_500_DEGS:
        writeRegister(GYRO_CONFIG, DPS500);
        break;
    case G_1000_DEGS:
        writeRegister(GYRO_CONFIG, DPS1000);
        break;
    case G_2000_DEGS:
        writeRegister(GYRO_CONFIG, DPS2000);
        break;
    }

    switch (accel_range)
    {
    case A_2_G:
        writeRegister(ACCEL_CONFIG, G2);
        break;
    case A_4_G:
        writeRegister(ACCEL_CONFIG, G4);
        break;
    case A_8_G:
        writeRegister(ACCEL_CONFIG, G8);
        break;
    case A_16_G:
        writeRegister(ACCEL_CONFIG, G16);
        break;
    }

    delay(100);

    return 0;
}

void ICM_20608_G::calibrate()
{
    const uint16_t number_messurements = 500;

    int16_t gx, gy, gz;
    int16_t ax, ay, az;

    int32_t sum_gx = 0, sum_gy = 0, sum_gz = 0;
    int32_t sum_ax = 0, sum_ay = 0, sum_az = 0;

    for (uint16_t i = 0; i < number_messurements; i++)
    {
        readGyroRaw(gx, gy, gz);
        readAccelRaw(ax, ay, az);

        sum_gx += gx;
        sum_gy += gy;
        sum_gz += gz;

        sum_ax += ax;
        sum_ay += ay;
        sum_az += az;

        delay(5);
    }

    offset_gx = -sum_gx / number_messurements;
    offset_gy = -sum_gy / number_messurements;
    offset_gz = -sum_gz / number_messurements;
    offset_ax = -sum_ax / number_messurements;
    offset_ay = -sum_ay / number_messurements;
    offset_az = 1 / accel_lsb_to_g - sum_az / number_messurements;
}

void ICM_20608_G::writeRegister(uint8_t reg, uint8_t val)
{
    SPI.beginTransaction(settings);
    digitalWrite(CS, LOW);

    SPI.transfer(reg & ~(1 << 7)); // MSB = 0 for Writing
    SPI.transfer(val);

    digitalWrite(CS, HIGH);
    SPI.endTransaction();
}

uint8_t ICM_20608_G::readRegister(uint8_t reg)
{
    SPI.beginTransaction(settings);
    digitalWrite(CS, LOW);

    SPI.transfer(reg | (1 << 7)); // MSB = 1 for Reading
    uint8_t val = SPI.transfer(0);

    digitalWrite(CS, HIGH);
    SPI.endTransaction();

    return val;
}

void ICM_20608_G::readGyroRaw(int16_t &x, int16_t &y, int16_t &z) const
{
    SPI.beginTransaction(settings);
    digitalWrite(CS, LOW);

    SPI.transfer(GYRO_XOUT_H | (1 << 7));
    x = ((int16_t)SPI.transfer(GYRO_XOUT_L | (1 << 7))) << 8;
    x |= SPI.transfer(GYRO_YOUT_H | (1 << 7));
    y = ((int16_t)SPI.transfer(GYRO_YOUT_L | (1 << 7))) << 8;
    y |= SPI.transfer(GYRO_ZOUT_H | (1 << 7));
    z = ((int16_t)SPI.transfer(GYRO_ZOUT_L | (1 << 7))) << 8;
    z |= SPI.transfer(0);

    digitalWrite(CS, HIGH);
    SPI.endTransaction();
}

void ICM_20608_G::readGyro(float &x, float &y, float &z) const
{
    int16_t xi, yi, zi;

    readGyroRaw(xi, yi, zi);

    x = -(float)(xi + offset_gx) * gyro_lsb_to_degs;
    y = -(float)(yi + offset_gy) * gyro_lsb_to_degs;
    z = (float)(zi + offset_gz) * gyro_lsb_to_degs;
}

void ICM_20608_G::readAccelRaw(int16_t &x, int16_t &y, int16_t &z) const
{
    SPI.beginTransaction(settings);
    digitalWrite(CS, LOW);

    SPI.transfer(ACCEL_XOUT_H | (1 << 7));
    x = ((int16_t)SPI.transfer(ACCEL_XOUT_L | (1 << 7))) << 8;
    x |= SPI.transfer(ACCEL_YOUT_H | (1 << 7));
    y = ((int16_t)SPI.transfer(ACCEL_YOUT_L | (1 << 7))) << 8;
    y |= SPI.transfer(ACCEL_ZOUT_H | (1 << 7));
    z = ((int16_t)SPI.transfer(ACCEL_ZOUT_L | (1 << 7))) << 8;
    z |= SPI.transfer(0);

    digitalWrite(CS, HIGH);
    SPI.endTransaction();
}

void ICM_20608_G::readAccel(float &x, float &y, float &z) const
{
    int16_t xi, yi, zi;

    readAccelRaw(xi, yi, zi);

    x = (float)(xi + offset_ax) * accel_lsb_to_g;
    y = (float)(yi + offset_ay) * accel_lsb_to_g;
    z = -(float)(zi + offset_az) * accel_lsb_to_g;
}