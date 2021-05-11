#include "estimator.h"
#include "stateMachine.h"


Estimator::Estimator(stateMachine* sm):
_sm(sm),
madgwick(0.5,5),
madgwick_delta_t(5)
{

}

void Estimator::setup(){
//nothing to do here 
};

void Estimator::update(){
   // int batt_voltage = _sm->sensors.sensors_raw.batt_volt; //example to get value
/*
    if (millis() - madgwick_prev_time >= madgwick_delta_t){
        float dt = millis() - madgwick_prev_time;
        madgwick.update(_sm->sensors.sensors_raw.gx,
                        _sm->sensors.sensors_raw.gy,
                        _sm->sensors.sensors_raw.gz,
                        _sm->sensors.sensors_raw.ax,
                        _sm->sensors.sensors_raw.ay,
                        _sm->sensors.sensors_raw.az,
                        -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                        _sm->sensors.sensors_raw.my,
                        _sm->sensors.sensors_raw.mz,
                        dt);
        madgwick_prev_time = millis();
    };*/
    madgwick.update(_sm->sensors.sensors_raw.gx,
                    _sm->sensors.sensors_raw.gy,
                    _sm->sensors.sensors_raw.gz,
                    _sm->sensors.sensors_raw.ax,
                    _sm->sensors.sensors_raw.ay,
                    _sm->sensors.sensors_raw.az,
                    -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                    _sm->sensors.sensors_raw.my,
                    _sm->sensors.sensors_raw.mz,
                    0.00210084); // sample time (s) based on gyro sample rate

    
    /*
    float dt = millis() - madgwick_prev_time;
    madgwick.update(_sm->sensors.sensors_raw.gx,
                    _sm->sensors.sensors_raw.gy,
                    _sm->sensors.sensors_raw.gz,
                    _sm->sensors.sensors_raw.ax,
                    _sm->sensors.sensors_raw.ay,
                    _sm->sensors.sensors_raw.az,
                    -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                    _sm->sensors.sensors_raw.my,
                    _sm->sensors.sensors_raw.mz,dt);
    madgwick_prev_time = millis();*/
    /*
    Serial.print("orientation :");
    
    Serial.print(madgwick.getRoll());
    Serial.print("\t");
    Serial.print(madgwick.getPitch());
    Serial.print("\t"); 
    Serial.print(madgwick.getYaw());
    Serial.print("\n");
    */
    /* 
    Serial.print(_sm->sensors.sensors_raw.gx);
    Serial.print("\t"); 
    Serial.print(_sm->sensors.sensors_raw.gy);
    Serial.print("\t");
    Serial.print(_sm->sensors.sensors_raw.gz);
    Serial.print("\t");
    Serial.print(_sm->sensors.sensors_raw.ax);
    Serial.print("\t"); 
    Serial.print(_sm->sensors.sensors_raw.ay);
    Serial.print("\t");
    Serial.print(_sm->sensors.sensors_raw.az);
    Serial.print("\t");
    Serial.print(_sm->sensors.sensors_raw.mx);
    Serial.print("\t"); 
    Serial.print(_sm->sensors.sensors_raw.my);
    Serial.print("\t");
    Serial.print(_sm->sensors.sensors_raw.mz);
    
    Serial.print("\n");  */
      
};