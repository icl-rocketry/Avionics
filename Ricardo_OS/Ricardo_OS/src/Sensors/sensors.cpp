#include "sensors.h"
#include "stateMachine.h"

#include <SPI.h>
#include <Wire.h>
#include <memory>
#include <functional>
#include <ArduinoJson.h>

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

#include "Storage/logController.h"
#include "Storage/systemstatus.h"

#include "Commands/commandpacket.h"

#include "Helpers/jsonconfighelper.h"


//config
#include "ricardo_pins.h"
#include "config.h"
#include "flags.h"

#include "packets/hitlpacket.h"
//indivudal sensor classes

#include "max_m8q.h"
#include "ms5607.h"
#include "icm_20608.h"
#include "h3lis331dl.h"

#include "battery.h"

Sensors::Sensors(SPIClass& spi,TwoWire& I2C,SystemStatus& systemstatus,LogController& logcontroller) :
    _systemstatus(systemstatus),
    gps(I2C,systemstatus,logcontroller),
    baro(spi,systemstatus,logcontroller,BaroCs),
    accelgyro(spi,systemstatus,logcontroller,ImuCs_1),
    accel(spi,systemstatus,logcontroller,ImuCs_2),
    mag(spi,systemstatus,logcontroller,MagCs),
    batt(systemstatus,logcontroller,BattVolt),
    logcontroller(logcontroller)
    
{}

void Sensors::setup(JsonObjectConst config){
    using namespace JsonConfigHelper;
    //default axes order and flip
    std::array<uint8_t,3> axesOrder{0,1,2};
    std::array<bool,3> axesFlip{0,0,0};

    setIfContains(config,"X_AXIS",axesOrder[0],false);
    setIfContains(config,"Y_AXIS",axesOrder[1],false);
    setIfContains(config,"Z_AXIS",axesOrder[2],false);
    setIfContains(config,"X_FLIP",axesFlip[0],false);
    setIfContains(config,"Y_FLIP",axesFlip[1],false);
    setIfContains(config,"Z_FLIP",axesFlip[2],false);

    gps.setup();
    baro.setup();
    accelgyro.setup(axesOrder,axesFlip);
    accel.setup(axesOrder,axesFlip);
    mag.setup(axesOrder,axesFlip);
    batt.setup();
    
};

void Sensors::update()
{
    if (_hitlEnabled){
        if (!_systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG))
        {
        _hitlEnabled = false;
        }
        return;
    }
    gps.update(sensors_raw.gps);
    baro.update(sensors_raw.baro);
    accelgyro.update(sensors_raw.accelgyro);
    accel.update(sensors_raw.accel);
    mag.update(sensors_raw.mag);
    batt.update(sensors_raw.batt);

};

const SensorStructs::raw_measurements_t& Sensors::getData()
{
    //TODO make this threadsafe maybe use a double buffer to make it lock free
    return sensors_raw;
}

void Sensors::calibrateAccelGyro()
{
    accelgyro.calibrateBias();
}

void Sensors::calibrateMag(MagCalibrationParameters magcal)
{
    mag.calibrate(magcal);
}

void Sensors::calibrateBaro()
{
    baro.calibrateBaro();
}

std::function<void(std::unique_ptr<RnpPacketSerialized>)> Sensors::getHitlCallback()
{
    return [this](std::unique_ptr<RnpPacketSerialized> packet_ptr){hitlHandler(std::move(packet_ptr));};
}

void Sensors::hitlHandler(std::unique_ptr<RnpPacketSerialized> packet_ptr)
{
    //final check that this has only be called in debug mode
    if (!_systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){
        return;
    }
    //process hitl packet
    switch (packet_ptr->header.type){
        case static_cast<uint8_t>(HITL_PACKET_TYPES::HITL_COMMAND):
        {
            hitlCommandHandler(*packet_ptr);
            return;
        }
        case static_cast<uint8_t>(HITL_PACKET_TYPES::PICKLE_RICK_SENSORS):
        {

            PickleRickSensorsPacket FakeData(*packet_ptr); // deserialize fake data
            sensors_raw.accelgyro.ax = FakeData.ax;
            sensors_raw.accelgyro.ay = FakeData.ay;
            sensors_raw.accelgyro.az = FakeData.az;
            sensors_raw.accelgyro.gx = FakeData.gx;
            sensors_raw.accelgyro.gy = FakeData.gy;
            sensors_raw.accelgyro.gz = FakeData.gz;

            sensors_raw.accel.ax = FakeData.h_ax;
            sensors_raw.accel.ay = FakeData.h_ay;
            sensors_raw.accel.az = FakeData.h_az;

            sensors_raw.mag.mx = FakeData.mx;
            sensors_raw.mag.my = FakeData.my;
            sensors_raw.mag.mz = FakeData.mz;

            sensors_raw.baro.alt = FakeData.baro_alt;
            sensors_raw.baro.temp = FakeData.baro_temp;
            sensors_raw.baro.press = FakeData.baro_press;

            sensors_raw.gps.lat = FakeData.gps_lat;
            sensors_raw.gps.lng = FakeData.gps_lng;
            sensors_raw.gps.alt = FakeData.gps_alt;

            sensors_raw.gps.v_n = FakeData.gps_v_n;
            sensors_raw.gps.v_e = FakeData.gps_v_e;
            sensors_raw.gps.v_d = FakeData.gps_v_d;

            sensors_raw.gps.sat = FakeData.gps_sat;
            sensors_raw.gps.pdop = FakeData.gps_pdop;
            sensors_raw.gps.fix = FakeData.gps_fix;
            sensors_raw.gps.updated = FakeData.gps_updated;
            sensors_raw.gps.valid = FakeData.gps_valid;

            hitlUpdateSensorError(FakeData.imu_error, SYSTEM_FLAG::ERROR_IMU);
            hitlUpdateSensorError(FakeData.haccel_error, SYSTEM_FLAG::ERROR_HACCEL);
            hitlUpdateSensorError(FakeData.mag_error, SYSTEM_FLAG::ERROR_MAG);
            hitlUpdateSensorError(FakeData.baro_error, SYSTEM_FLAG::ERROR_BARO);
            hitlUpdateSensorError(FakeData.gps_error, SYSTEM_FLAG::ERROR_GPS);

            sensors_raw.system_time = millis();

            return;
        }
        default:
        {
            return;
        }
    }
  

}

void Sensors::hitlCommandHandler(RnpPacketSerialized& packet)
{
    switch(CommandPacket::getCommand(packet)){
        case static_cast<uint8_t>(HITL_COMMANDS::HITL_ENABLE):
        {
            _hitlEnabled = true;
            logcontroller.log("HITL Enabled!");
            return;
        }
        case static_cast<uint8_t>(HITL_COMMANDS::HITL_DISABLE):
        {
            _hitlEnabled = false;
            logcontroller.log("HITL Disabled!");
            return;
        }
        default:
        {
            return;
        }
    }
}

void Sensors::hitlUpdateSensorError(uint8_t sensor_state,SYSTEM_FLAG flag)
{
    if (sensor_state && !_systemstatus.flag_triggered(flag))
    {
        _systemstatus.new_message(flag, "hitl raised error");
    }
    else if (!sensor_state && _systemstatus.flag_triggered(flag))
    {
        _systemstatus.delete_message(flag, "hitl removed flag");
    }
}


