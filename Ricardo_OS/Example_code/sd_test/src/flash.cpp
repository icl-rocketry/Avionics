// The MIT License (MIT)
// Copyright (c) 2019 Ha Thach for Adafruit Industries

#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

#include "flash_devices.h"//flash_device configuration defintion

#include "SPI.h"


SPIClass vspi(VSPI);//spi object definition

Adafruit_FlashTransport_SPI flashTransport(15, vspi);
Adafruit_SPIFlash flash(&flashTransport);



// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(100);   // wait for native usb

  Serial.println("Adafruit Serial Flash Info example");
  SPIFlash_Device_t flash_config = W25Q128JV_PM; // retrieve flash specific config
  flash.begin(&flash_config); // pass config into begin function

  Serial.print("JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: "); Serial.print(flash.size() / 1024); Serial.println(" KB"); // get size
}

void loop()
{
  // nothing to do
}
