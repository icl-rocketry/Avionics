#include "i2cpyro.h"

#include "rocketactuator.h"
#include "rocketcomponenttype.h"

#include "Storage/logController.h"

#include <esp_task.h>


#include <functional>

#include <Wire.h>

I2CPyro::I2CPyro(uint8_t id,LogController& logcontroller,uint8_t address,uint8_t channel,bool continuityPolarity,TwoWire &wire):
RocketActuator(id,RocketComponentTypes::TYPES::I2C_ACT_PYRO,logcontroller),
_address(address),
_channel(channel),
pinsValid(true),
_nukePin(get_nuke_pin(channel)),
_continuityPin(get_continuity_pin(channel)),
_continuityPolarity(continuityPolarity),
_wire(wire)
{
    if (!pinsValid){
        _state.state = static_cast<uint8_t>(COMPONENT_STATE::ERROR);
        return;
    }
    // get port configuration
    uint8_t config = get_register(CONFIG);
    //update config
    _wire.beginTransmission(_address);
    _wire.write(CONFIG);
    //configure nuke pin as output
    config &= ~((1 << _nukePin));
    //configure cont pin as input
    config |= (1 << _continuityPin);
    //write new config
    _wire.write(config);
    _wire.endTransmission();

    //get polarity
    uint8_t polarity = get_register(POLARITY);
    
    _wire.beginTransmission(_address);
    _wire.write(POLARITY);
    if (_continuityPolarity) // invert input
    {
        polarity |= (1 << _continuityPin);
    }else{
        polarity &= ~ (1 << _continuityPin);
    }
    _wire.write(polarity);
    _wire.endTransmission();

    //make sure nuke pin is off
    uint8_t output = get_register(OUTPUT_PORT);
    _wire.beginTransmission(_address);
    _wire.write(OUTPUT_PORT);
    _wire.write(output &= ~(1 << _nukePin));
    _wire.endTransmission();

};


void I2CPyro::execute(int32_t param)
{
    if (!pinsValid){
        return;
    }
    //switch on nuke pin
    uint8_t output = get_register(OUTPUT_PORT);
    _wire.beginTransmission(_address);
    _wire.write(OUTPUT_PORT);
    _wire.write(output |= (1 << _nukePin));
    _wire.endTransmission();

    struct TaskData_t{
        const uint8_t address;
        const uint8_t nukePin;
        const int32_t param;
        TwoWire& wire;
    };

    // initialize new task data
    TaskData_t taskdata{_address, _nukePin, param, _wire};

    // spawn task to switch off pin given timeout param wiht higher prioirty so sleeping starts immediatley and non static task data is copied immediatley
    xTaskCreatePinnedToCore([](void *pvParameters)
                            {
                                // create local copy of data as task data is not static
                                TaskData_t taskdata = *reinterpret_cast<TaskData_t *>(pvParameters);

                                TickType_t xLastWakeTime = xTaskGetTickCount();
                                vTaskDelayUntil(&xLastWakeTime, taskdata.param / portTICK_PERIOD_MS); // sleep for required amount
                                uint8_t output = 0;

                                taskdata.wire.beginTransmission(taskdata.address);
                                taskdata.wire.write(OUTPUT_PORT);
                                taskdata.wire.endTransmission();

                                taskdata.wire.requestFrom(taskdata.address, static_cast<uint8_t>(1));
                                if (taskdata.wire.available())
                                {
                                    output = taskdata.wire.read();
                                }

                                taskdata.wire.beginTransmission(taskdata.address);
                                taskdata.wire.write(OUTPUT_PORT);
                                taskdata.wire.write(output &= ~(1 << taskdata.nukePin));
                                taskdata.wire.endTransmission();

                                vTaskDelete(NULL); // delete task 
                            },
                            "nukeofftask",
                            1000,
                            (void *)&taskdata,
                            2,
                            NULL,
                            1);
};

void I2CPyro::updateState()
{
    //read continuity and update state
    if (!pinsValid){
        return;
    }
    _state.currentValue = get_register(INPUT_PORT) & ( 1 << _continuityPin);
};


uint8_t I2CPyro::get_register(uint8_t reg)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.endTransmission();

    _wire.requestFrom(_address,1);
    if (_wire.available()){
        if (_state.state != static_cast<uint8_t>(COMPONENT_STATE::NOMINAL)){
            _state.state = static_cast<uint8_t>(COMPONENT_STATE::NOMINAL);
        }
        return _wire.read();
    }else{
        _state.state = static_cast<uint8_t>(COMPONENT_STATE::ERROR);
        return 0;
    }
};


uint8_t I2CPyro::get_nuke_pin(uint8_t channel)
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

uint8_t I2CPyro::get_continuity_pin(uint8_t channel)
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