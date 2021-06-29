#include "estimator.h"
#include "stateMachine.h"
#include "math.h"
#include "Storage/utils.h"
#include "flags.h"


Estimator::Estimator(stateMachine* sm):
_sm(sm),
update_frequency(5000),//200Hz update
madgwick(0.5,0.00210084) // beta | gyroscope sample time step (s)
{};

void Estimator::setup(){
//update board orientation this is applied when converthing back to sensor frame where the orientaiton of sensor matters
//upside down should be retireved from config file
   if (upsideDown){
      flipConstant = -1;
   }else{
      flipConstant = 1;
   }
   
};

void Estimator::update(){
   

   unsigned long dt = (unsigned long)(micros() - last_update); //explictly casting to prevent micros() overflow cuasing issues
   

   if (dt > update_frequency){

      last_update = micros(); // update last_update 
      float dt_seconds = float(dt)*0.000001F; //conversion to seconds

      // if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
      //    //no data so we cant calculate any nav solution
      //    if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::NOSOLUTION)){ // check if we already have logged this
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::NOSOLUTION);
      //       _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"no data, cannot compute navigation solution");
      //    }
      //    return;
      // }

      // if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_IMU)){
      //    //only gps and baro data -> update using gps and baro estimates
      //    if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::NOSOLUTION)){ // check if we already have logged this
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::NOSOLUTION);
      //       _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"no data, cannot compute navigation solution");
      //    }
      //    return;
      // }

      updateAngularRates();
      updateOrientation(dt_seconds);
      updateLinearAcceleration();

      // if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS) && _sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
      //    //only imu data available so cannot perform pose esitmation
      //    if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::ORIENTATION)){
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::ORIENTATION);
      //       _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"no gps or baro data, cannot compute location solution");
      //    }
      //    return;
      // }
      
      // if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_GPS)){
      //    //only imu and baro data available so cannot perform pose esitmation
      //    // update only baro measurement part of kf
      //    //
      //    //
      //    if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::PARTIAL_BARO)){
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::PARTIAL_BARO);
      //       _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"no gps data, cannot fully compute location solution");
      //    }
      //    return;
      // }

      // if (_sm->systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BARO)){
      //    //only imu and gps data available so cannot perform pose esitmation
      //    // update only gps measurement part of kf here
      //    //
      //    //
      //    if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::PARTIAL_GPS)){
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::PARTIAL_GPS);
      //       _sm->systemstatus.new_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"no baro data, cannot fully compute location solution");
      //    }
      //    return;
      // }

      // //full mesurement function


      // if (state.estimator_state != static_cast<uint8_t>(ESTIMATOR_STATE::NOMINAL)){
      //       state.estimator_state = static_cast<uint8_t>(ESTIMATOR_STATE::NOMINAL);
      //       _sm->systemstatus.delete_message(SYSTEM_FLAG::ERROR_ESTIMATOR,"Full solution avaliable");
      // }


   };
    
};

void Estimator::setHome(){
   // record current gps coordinates as home
   state.gps_launch_lat = _sm->sensors.sensors_raw.gps_lat;
   state.gps_launch_long = _sm->sensors.sensors_raw.gps_long;
   state.gps_launch_alt = _sm->sensors.sensors_raw.gps_alt;
   //log the new home position
   _sm->logcontroller.log("Home Position Updated to Lat: " 
                           + utils::tostring(state.gps_launch_lat)
                           + " Long: "
                           + utils::tostring(state.gps_launch_long)
                           + " Alt: "
                           + utils::tostring(state.gps_launch_alt));
}

void Estimator::updateLinearAcceleration(){
   //LINEAR ACCELERATION CALCULATION//
   //add raw accelerations into matrix form -> acceleration values in g's
   Eigen::Vector3f raw_accel(_sm->sensors.sensors_raw.ax,_sm->sensors.sensors_raw.ay,_sm->sensors.sensors_raw.az);
   
   //calculate linear acceleration in NED frame
   state.acceleration = (madgwick.getInverseRotationMatrix()*raw_accel) + gravity_vector;
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
