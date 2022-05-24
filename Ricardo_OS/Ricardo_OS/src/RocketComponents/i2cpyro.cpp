#include "i2cpyro.h"

#include "rocketactuator.h"
#include "rocketcomponenttype.h"

#include "Storage/logController.h"

#include <Wire.h>

I2CPyro::I2CPyro(uint8_t id,LogController& logcontroller,uint8_t address,uint8_t channel,TwoWire &wire):
RocketActuator(id,RocketComponentTypes::TYPES::I2C_ACT_PYRO,logcontroller),
_address(address),
_channel(channel),
pinsValid(true),
_nukePin(get_nuke_pin(channel)),
_contPin(get_cont_pin(channel)),
_wire(wire)
{
    //send presets to pca9534
};









uint8_t get_nuke_pin(uint8_t channel)
{
    if (channel <= 3)
    {
        return 7 - (2 * channel);
    }
    else
    {
        pinsValid = false;
        return 0;
    }
};

uint8_t get_cont_pin(uint8_t channel)
{
    if (channel <= 3)
    {
        return 6 - (channel * 2);
    }
    else
    {
        pinsValid = false;
        return 0;
    }
};