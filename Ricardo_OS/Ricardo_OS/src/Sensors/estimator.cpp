#include "estimator.h"
#include "stateMachine.h"
#include "math.h"


Estimator::Estimator(stateMachine* sm):
_sm(sm),
update_frequency(5000),//200Hz update
madgwick(0.5,0.00210084) // beta | gyroscope sample time step (s)
{};

void Estimator::setup(){
//nothing to do here 
};

void Estimator::update(){
   

   unsigned long dt = (unsigned long)(micros() - last_update); //explictly casting to prevent micros() overflow cuasing issues
   

   if (dt > update_frequency){

      last_update = micros(); // update last_update 
      float dt_seconds = float(dt)*0.000001F; //conversion to seconds
      

      updateAngularRates();
      updateOrientation("pid_results11.csv");
      updateLinearAcceleration();
      
   };


    

      
};

void Estimator::updateLinearAcceleration(){
   //LINEAR ACCELERATION CALCULATION//
   //add raw accelerations into matrix form -> acceleration values in g's
   Eigen::Vector3f raw_accel(_sm->sensors.sensors_raw.ax,_sm->sensors.sensors_raw.ay,_sm->sensors.sensors_raw.az);
   if (upsideDown){
      flipConstant = -1;
   }else{
      flipConstant = 1;
   }
   //calculate linear acceleration in NED frame
   state.acceleration = (madgwick.getInverseRotationMatrix()*raw_accel) + (flipConstant*gravity_vector);
};

void Estimator::updateAngularRates(){
   //update angular rates
   state.angularRates = Eigen::Vector3f{_sm->sensors.sensors_raw.gx,
                                       _sm->sensors.sensors_raw.gy,
                                       _sm->sensors.sensors_raw.gz};
};

void Estimator::updateOrientation(float dt){
   //calculate orientation solution
   madgwick.setDeltaT(dt); // update integration time
   madgwick.update(_sm->sensors.sensors_raw.gx,
                  _sm->sensors.sensors_raw.gy,
                  _sm->sensors.sensors_raw.gz,
                  _sm->sensors.sensors_raw.ax,
                  _sm->sensors.sensors_raw.ay,
                  _sm->sensors.sensors_raw.az,
                  -(_sm->sensors.sensors_raw.mx), // lsm9ds1 magnetometer x axis is opposite to gyro and accel
                  _sm->sensors.sensors_raw.my,
                  _sm->sensors.sensors_raw.mz); 
   //update orientation
   state.orientation = madgwick.getOrientation();
   state.eulerAngles = madgwick.getEulerAngles();
};
