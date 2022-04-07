# ICM-20608-G

This library lets you interface an ICM-20608-G IMU via SPI.

## Usage

```C++
#include "ICM-20608-G.h"

// see the header files for available ranges
ICM_20608_G imu(GyroRange::G_500_DEGS, AccelRange::A_4_G);

void setup()
{
    SPI.begin(); // start the SPI
    delay(10);
    imu.init();
    imu.calibrate(); // only do this if your z-axis points upwards
}

void loop()
{
    float gx, gy, gz, ax, ay, az;

    imu.readGyro(gx, gy, gz);
    imu.readAccel(ax, ay, az);

    // do something

    delay(1);
}
```

### Calibration

Calibration in this case just means taking a few measurements and then using the mean as offset values. In order to work correctly, the IMU should not be moved and the z axis should point upwards.