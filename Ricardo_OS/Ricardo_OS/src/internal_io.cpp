#include "ricardo_pins.h"
#include "config.h"
#include "internal_io.h"

void setup_io(){
    //intialize i2c interface
    I2C.begin(SDA,SCL,I2C_FREQUENCY);
    //initalize spi interface
        //todo find the lowest denominator for the speed of the spi bus.
    //vspi.setClockDivider(SPI_CLOCK_DIV8);
    vspi.begin();
    //setup cs pins
        //initialise output variables as output
    pinMode(LoraCs, OUTPUT);
    pinMode(ImuCs, OUTPUT);
    pinMode(BaroCs, OUTPUT);
    pinMode(MagCs, OUTPUT);
    pinMode(FlashCs, OUTPUT);
    pinMode(SdCs, OUTPUT);
        //initialise outputs as high
    digitalWrite(LoraCs, HIGH);
    digitalWrite(ImuCs, HIGH);
    digitalWrite(BaroCs, HIGH);
    digitalWrite(MagCs, HIGH);
    digitalWrite(FlashCs, HIGH);
    digitalWrite(SdCs, HIGH);
    //open serial port on usb interface
    Serial.begin(115200);

};