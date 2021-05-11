#include "estimator.h"
#include "stateMachine.h"


Estimator::Estimator(stateMachine* sm):
_sm(sm),
madgwick(0.5,0.00210084) // beta | gyroscope sample time step (s)
{};

void Estimator::setup(){
//nothing to do here 
};

void Estimator::update(){
   // int batt_voltage = _sm->sensors.sensors_raw.batt_volt; //example to get value

    madgwick.update(_sm->sensors.sensors_raw.gx,
                    _sm->sensors.sensors_raw.gy,
                    _sm->sensors.sensors_raw.gz,
                    _sm->sensors.sensors_raw.ax,
                    _sm->sensors.sensors_raw.ay,
                    _sm->sensors.sensors_raw.az,
                    -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                    _sm->sensors.sensors_raw.my,
                    _sm->sensors.sensors_raw.mz); 

    

      
};